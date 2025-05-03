//
//  rle_ac.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "rle_ac.h"
#include <cmath>

namespace RLE {

// Преобразует целое число в битовую строку фиксированной длины.
static std::string intToBinary(unsigned int value, int bits) {
    std::string result;
    for (int i = bits - 1; i >= 0; i--) {
        result.push_back(((value >> i) & 1) ? '1' : '0');
    }
    return result;
}

// Преобразует строку битов в целое число.
static unsigned int binaryToInt(const std::string &bits) {
    unsigned int value = 0;
    for (char bit : bits) {
        value = (value << 1) + (bit - '0');
    }
    return value;
}

// Определяет категорию числа: количество бит, необходимых для представления его модуля.
static int getCategory(int value) {
    if (value == 0)
        return 0;
    int absVal = std::abs(value);
    return static_cast<int>(std::floor(std::log2(absVal))) + 1;
}

// Функция для RLE‑кодирования AC коэффициентов.
std::string encodeAC(const std::vector<int>& acCoeffs) {
    std::string bitstream;
    int zeroCount = 0;
    for (size_t i = 0; i < acCoeffs.size(); i++) {
        int coeff = acCoeffs[i];
        if (coeff == 0) {
            zeroCount++;
            if (zeroCount == 16) {
                // ZRL: специальный код для 16 последовательных нулей.
                bitstream += "11110000";
                zeroCount = 0;
            }
        } else {
            int category = getCategory(coeff);
            // Формирование 8‑битного поля: 4 бита для количества накопленных нулей и 4 бита для категории.
            unsigned int firstByte = ((zeroCount & 0xF) << 4) | (category & 0xF);
            bitstream += intToBinary(firstByte, 8);
            // Добавляем дополнительные биты при необходимости (если category > 0)
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
    // Добавляем завершающий код EOB – сигнал конца блока
    bitstream += "00000000";
    return bitstream;
}

// Функция для декодирования RLE‑кодированных AC коэффициентов.
std::vector<int> decodeAC(const std::string& bitstream) {
    std::vector<int> acCoeffs;
    size_t index = 0;
    while (index + 8 <= bitstream.size()) {
        std::string byteStr = bitstream.substr(index, 8);
        index += 8;
        // Если найден код EOB ("00000000"), завершаем декодирование.
        if (byteStr == "00000000") {
            break;
        }
        // Если обнаружен ZRL ("11110000") – добавляем 16 нулей.
        if (byteStr == "11110000") {
            for (int i = 0; i < 16; i++) {
                acCoeffs.push_back(0);
            }
            continue;
        }
        // Извлекаем run length (количество нулей) и категорию из 8‑битного поля.
        int runLength = static_cast<int>(binaryToInt(byteStr.substr(0, 4)));
        int category   = static_cast<int>(binaryToInt(byteStr.substr(4, 4)));
        // Добавляем указанное количество нулей.
        for (int i = 0; i < runLength; i++) {
            acCoeffs.push_back(0);
        }
        int coeff = 0;
        if (category > 0) {
            if(index + category > bitstream.size()) break;  // Если недостаточно бит для чтения.
            std::string additionalStr = bitstream.substr(index, category);
            index += category;
            unsigned int additional = binaryToInt(additionalStr);
            int threshold = 1 << (category - 1);
            // Если дополнительное значение меньше порогового – восстанавливаем отрицательное число.
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

} // namespace RLE

