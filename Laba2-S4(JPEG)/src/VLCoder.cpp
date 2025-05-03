//
//  VLCoder.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "VLCoder.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>

// Вспомогательная функция: преобразование целого числа в строку битов фиксированной длины.
static std::string intToBinary(unsigned int value, int bits) {
    std::string result;
    for (int i = bits - 1; i >= 0; i--) {
        result.push_back(((value >> i) & 1) ? '1' : '0');
    }
    return result;
}

// Вспомогательная функция: преобразование строки битов в целое число.
static unsigned int binaryToInt(const std::string &bits) {
    unsigned int value = 0;
    for (char bit : bits) {
        value = (value << 1) + (bit - '0');
    }
    return value;
}

// Определение категории для числа (количество бит для дополнительного представления)
static int getCategory(int value) {
    if (value == 0)
        return 0;
    int absVal = std::abs(value);
    return static_cast<int>(std::floor(std::log2(absVal))) + 1;
}

namespace VLCoder {

    // Функция для переменного кодирования DC коэффициентов.
    // Для каждого разностного значения:
    //  - сначала записывается 4‑битное поле, представляющее категорию;
    //  - если категория > 0, затем записываются additional bits: для положительного числа – двоичный вид,
    //    для отрицательного – число, сдвинутое по формуле: code = diff + ((1 << category) – 1).
    std::string encodeDC(const std::vector<int>& dcDiffs) {
        std::string bitstream;
        for (int diff : dcDiffs) {
            int category = getCategory(diff);
            // Записываем 4 бита категории
            bitstream += intToBinary(category, 4);
            if (category > 0) {
                unsigned int additional;
                if (diff < 0) {
                    additional = static_cast<unsigned int>(diff + ((1 << category) - 1));
                } else {
                    additional = static_cast<unsigned int>(diff);
                }
                // Записываем дополнительное представление в category битах
                bitstream += intToBinary(additional, category);
            }
        }
        return bitstream;
    }

    // Функция для декодирования DC коэффициентов из битового потока.
    // Считывает сначала 4‑битное поле для категории, затем при category > 0 считывает category бит.
    // Если дополнительное число меньше порогового значения (1 << (category - 1)), значение корректируется.
    std::vector<int> decodeDC(const std::string& bitstream) {
        std::vector<int> dcDiffs;
        size_t index = 0;
        while (index + 4 <= bitstream.size()) {
            // Считываем 4 бита категории
            std::string catStr = bitstream.substr(index, 4);
            index += 4;
            int category = static_cast<int>(binaryToInt(catStr));
            int diff = 0;
            if (category > 0) {
                if(index + category > bitstream.size()) break;  // Недостаточно бит для чтения
                std::string additionalStr = bitstream.substr(index, category);
                index += category;
                unsigned int additional = binaryToInt(additionalStr);
                int threshold = 1 << (category - 1);
                if (additional < static_cast<unsigned int>(threshold)) {
                    diff = static_cast<int>(additional) - ((1 << category) - 1);
                } else {
                    diff = static_cast<int>(additional);
                }
            }
            dcDiffs.push_back(diff);
        }
        return dcDiffs;
    }

    // Функция для переменного кодирования AC коэффициентов с run-length кодированием.
    // Алгоритм:
    //  - Последовательно подсчитываются нули.
    //  - Если нулей >= 16, выводится специальный код ZRL: "11110000".
    //  - При обнаружении ненулевого коэффициента сначала выводится 8‑битное поле:
    //       первые 4 бита – run length (число предыдущих нулей),
    //       вторые 4 бита – категория ненулевого коэффициента.
    //    Затем, если категория > 0, записываются дополнительные биты (длина = category),
    //    кодирующие значение коэффициента (аналогично DC).
    //  - После завершения блока выводится EOB – 8‑битное поле "00000000".
    std::string encodeAC(const std::vector<int>& acCoeffs) {
        std::string bitstream;
        int zeroCount = 0;
        for (size_t i = 0; i < acCoeffs.size(); i++) {
            int coeff = acCoeffs[i];
            if (coeff == 0) {
                zeroCount++;
                if (zeroCount == 16) {
                    // ZRL код, сбрасываем счетчик
                    bitstream += "11110000";
                    zeroCount = 0;
                }
            } else {
                // Если ранее были нули, то их run length и категория кодируются в одном байте.
                int category = getCategory(coeff);
                // Формируем 8‑битное поле: первые 4 бита – run length, вторые 4 бита – категория.
                unsigned int firstByte = ((zeroCount & 0xF) << 4) | (category & 0xF);
                bitstream += intToBinary(firstByte, 8);
                if (category > 0) {
                    unsigned int additional;
                    if (coeff < 0) {
                        additional = static_cast<unsigned int>(coeff + ((1 << category) - 1));
                    } else {
                        additional = static_cast<unsigned int>(coeff);
                    }
                    bitstream += intToBinary(additional, category);
                }
                zeroCount = 0;
            }
        }
        // По окончании блока кодируем оставшиеся нули (если таковые есть)
        // Согласно спецификации, в конце блока выводится символ EOB "00000000"
        bitstream += "00000000";
        return bitstream;
    }

    // Функция для декодирования AC коэффициентов из битового потока.
    // Считывает 8‑битные символы, если это ZRL ("11110000") – добавляет 16 нулей.
    // Иначе, первые 4 бита (run length) определяют количество нулей, вторые 4 бита – категорию.
    // При category > 0, затем считывается additional bits (длина = category)
    // и коэффициент восстанавливается аналогично DC.
    std::vector<int> decodeAC(const std::string& bitstream) {
        std::vector<int> acCoeffs;
        size_t index = 0;
        while (index + 8 <= bitstream.size()) {
            std::string byteStr = bitstream.substr(index, 8);
            index += 8;
            // EOB: если найден код "00000000", завершаем декодирование
            if (byteStr == "00000000") {
                break;
            }
            // ZRL: "11110000" означает 16 нулей
            if (byteStr == "11110000") {
                for (int i = 0; i < 16; i++) {
                    acCoeffs.push_back(0);
                }
                continue;
            }
            // Первые 4 бита – run length, вторые 4 бита – категория
            int runLength = static_cast<int>(binaryToInt(byteStr.substr(0, 4)));
            int category = static_cast<int>(binaryToInt(byteStr.substr(4, 4)));
            // Добавляем указанные количества нулей
            for (int i = 0; i < runLength; i++) {
                acCoeffs.push_back(0);
            }
            int coeff = 0;
            if (category > 0) {
                if(index + category > bitstream.size()) break; // недостаточно бит
                std::string additionalStr = bitstream.substr(index, category);
                index += category;
                unsigned int additional = binaryToInt(additionalStr);
                int threshold = 1 << (category - 1);
                if (additional < static_cast<unsigned int>(threshold)) {
                    coeff = static_cast<int>(additional) - ((1 << category) - 1);
                } else {
                    coeff = static_cast<int>(additional);
                }
            }
            acCoeffs.push_back(coeff);
        }
        return acCoeffs;
    }

} // namespace VLCoder
