#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

/*


//no use
std::vector<uint8_t> dynamic_len_encode_3byte(long number, bool firstBit) { //no use
    std::vector<uint8_t> result;

    if (number < 0) {
        throw std::invalid_argument("Number must be non-negative");
    }

    // Первый байт
    uint8_t firstByte = (firstBit ? 0x80 : 0x00); // Устанавливаем старший бит
    firstByte |= (number & 0x3F); // Устанавливаем 6 младших бит
    number >>= 6;

    // Если число не поместилось в первый байт
    if (number > 0) {
        firstByte |= 0x40; // Устанавливаем второй старший бит (есть еще байт)
        result.push_back(firstByte);

        // Второй байт
        uint8_t secondByte = (number & 0x7F); // Устанавливаем 7 младших бит
        number >>= 7;

        // Если число не поместилось во второй байт
        if (number > 0) {
            secondByte |= 0x80; // Устанавливаем старший бит (есть еще байт)
            result.push_back(secondByte);

            // Третий байт
            uint8_t thirdByte = (number & 0xFF); // Устанавливаем все 8 бит
            result.push_back(thirdByte);
        } else {
            result.push_back(secondByte);
        }
    } else {
        result.push_back(firstByte);
    }

    return result;
}

*/

std::vector<uint8_t> dynamic_len_encode_2byte(long number, bool firstBit) {
    std::vector<uint8_t> result;

    if (number < 0) {
        throw std::invalid_argument("Number must be non-negative");
    }

    // Первый байт
    uint8_t firstByte = (firstBit ? 0x80 : 0x00); // Устанавливаем старший бит
    firstByte |= (number & 0x3F); // Устанавливаем 6 младших бит
    number >>= 6;

    // Если число не поместилось в первый байт
    if (number > 0) {
        firstByte |= 0x40; // Устанавливаем второй старший бит (есть второй байт)
        result.push_back(firstByte);

        // Второй байт
        uint8_t secondByte = (number & 0xFF); // Устанавливаем все 8 бит
        result.push_back(secondByte);
    } else {
        result.push_back(firstByte);
    }

    return result;
}

std::vector<uint8_t> dynamic_len_encode_1byte(long number, bool firstBit) {
    std::vector<uint8_t> result;

    // Проверка на допустимость числа
    if (number < 0) {
        throw std::invalid_argument("Number must be non-negative");
    }

    // Формирование первого байта
    uint8_t firstByte = (firstBit ? 0x80 : 0x00); // Устанавливаем старший бит
    firstByte |= (number & 0x7F); // Устанавливаем 7 младших бит

    result.push_back(firstByte);

    return result;
}

std::pair<bool, long> dynamic_len_decode_2byte(const std::vector<uint8_t>& bytes) {
    if (bytes.empty()) {
        throw std::invalid_argument("Input bytes array is empty");
    }

    // Первый байт
    uint8_t firstByte = bytes[0];
    bool firstBit = (firstByte & 0x80) != 0;
    long number = firstByte & 0x3F;
    int shift = 6;

    // Если есть второй байт
    if (firstByte & 0x40) {
        if (bytes.size() < 2) {
            throw std::invalid_argument("Missing second byte");
        }
        uint8_t secondByte = bytes[1];
        number |= (static_cast<long>(secondByte) << shift);
    }

    return {firstBit, number};
}

std::pair<long, bool> dynamic_len_decode_1byte(const std::vector<uint8_t>& bytes)
{
    bool firstBit = (bytes[0] & 0x80) != 0;

    long number = bytes[0] & 0x7F;

    return {number, firstBit};
}

class RlePackBits {
public:
    std::string encode(const std::string& block) {
    std::string encoded;
    size_t i = 0;

    while (i < block.size()) {
        char byte = block[i];
        size_t count = 1;


        while (i + 1 < block.size() && block[i + 1] == byte) {
            count++;
            i++;
        }

        if (count > 1) {
            long flag=0;;
             // повторы
            while (count > 0){
                size_t chunk_length = std::min(static_cast<size_t>(16382), count); //(16382) //(2097150)
                
                std::vector<uint8_t> len_code = dynamic_len_encode_2byte(chunk_length,false);
                
                for (auto byte : len_code) {
                    encoded.push_back(static_cast<char>(byte));
                }
                
                encoded.push_back(byte);
                
                count=count-chunk_length;
                
                flag++;
                
            }
            if(flag>1){
                std::cout<<"Закодировали (четнное) раз:"<<flag<<std::endl;
            }
            
            
        } else { //повторы
            long flag=0;
            
            size_t start = i;
            
            while (i + 2 < block.size() && block[i + 1] != block[i] && block[i + 2] != block[i+1]) {
                i++;
            }
            
            size_t length = i - start + 1;
            

            // одиночки
            while (length > 0) {
                size_t chunk_length = std::min(static_cast<size_t>((127)), length); //(16382)
                
               std::vector<uint8_t> len_code = dynamic_len_encode_1byte(chunk_length,true);
                
                for (auto byte : len_code) {
                    encoded.push_back(static_cast<char>(byte));
                }
               
                for (size_t j = 0; j < chunk_length; j++) {
                    encoded.push_back(block[start + j]);
                }
                
                start += chunk_length;
                length -= chunk_length;
                
                flag++;
            }
            if(flag>1){
                std::cout<<"Закодировали (нечетное) раз:"<<flag<<std::endl;
            }
        }
        i++;
    }
    return encoded;
}

    std::string decode(const std::string& block) {
        std::string decoded;
        size_t i = 0;

        while (i < block.size()) {
            
            std::vector<uint8_t> result;
            result.clear();
            
            int8_t counter = static_cast<int8_t>(block[i]);
            result.push_back(counter);
            
            // Проверяем второй старший бит первого байта
            if (((result[0] & 0b01000000) != 0) && ((result[0] & 0b10000000) == 0)) { // если второй старший бит = 1
                i++;
                int8_t nextByte = static_cast<uint8_t>(block[i]);
                result.push_back(nextByte);
            }
            
            if((result[0] & 0b10000000) != 0){
                auto bytes_decoded= dynamic_len_decode_1byte(result);
                
                long len = bytes_decoded.first;
                
                for (long j = 0; j < len; j++) {
                    decoded.push_back(block[i + 1 + j]);
                }
                i += 1 + len;
                
            }else{
                auto bytes_decoded= dynamic_len_decode_2byte(result);
                
                long len = bytes_decoded.second;
                
                
                for (long j = 0; j < len; j++) {
                    decoded.push_back(block[i + 1]);
                }
                i += 2;
            }
            
        }
        return decoded;
    }
};


//бинарный ридер
std::string readFileAsString(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Ошибка: не удалось открыть файл " + filename);
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string content(size, '\0');
    if (!file.read(&content[0], size)) {
        throw std::runtime_error("Ошибка: не удалось прочитать файл " + filename);
    }

    return content;
}


//DEBUG
/*
int main() {
    // Чтение строки из файла
    std::ofstream outRLE("/Users/Egor 1/Desktop/КРАСИН/RLE-test-encoded.txt");
    std::string filename = "/Users/Egor 1/Desktop/КРАСИН/enwik7-test — output.txt";
    std::string input = readFileAsString(filename);
    std::ofstream outDecoded("/Users/Egor 1/Desktop/КРАСИН/RLE-test-decoded.txt");

    RlePackBits rle;
    
    // Начало замера времени
    auto start = std::chrono::high_resolution_clock::now();

    // Кодирование и декодирование
    std::string encoded = rle.encode(input);
    std::string decoded = rle.decode(encoded);

    // Конец замера времени
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    outRLE << encoded << std::endl;
    outDecoded << decoded << std::endl;

    // Вывод времени выполнения
    std::cout << "Время выполнения: " << duration.count() << " секунд." << std::endl;

    return 0;
}

*/
