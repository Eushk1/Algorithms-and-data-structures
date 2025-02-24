#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <tuple>
#include <numeric>
#include <stdexcept>

//using byte = uint8_t;


using byte = unsigned char;


std::pair<std::vector<byte>, size_t> bwt_encode(const std::vector<byte>& input) {
    size_t n = input.size();
    if (n == 0)
        return {{}, 0};

    std::vector<size_t> indices(n);
    std::iota(indices.begin(), indices.end(), 0);

    auto compare = [&](size_t i, size_t j) {
        for (size_t k = 0; k < n; ++k) {
            byte a = input[(i + k) % n];
            byte b = input[(j + k) % n];
            if (a != b)
                return a < b;
        }
        return false;
    };

    std::sort(indices.begin(), indices.end(), compare);

    std::vector<byte> encoded(n);
    size_t originalIndex = 0;
    for (size_t rank = 0; rank < n; ++rank) {
        size_t index = indices[rank];
        encoded[rank] = input[(index + n - 1) % n];
        if (index == 0) {
            originalIndex = rank;
        }
    }

    return {encoded, originalIndex};
}


std::vector<byte> bwt_decode(const std::vector<byte>& bwt, size_t originalIndex) {
    size_t n = bwt.size();
    if(n == 0)
        return {};

    std::vector<byte> first(bwt);
    std::sort(first.begin(), first.end());

    std::vector<size_t> occ(n, 0);
    
    std::vector<size_t> frequency(256, 0);
    
    for (size_t i = 0; i < n; ++i) {
        occ[i] = frequency[bwt[i]];
        frequency[bwt[i]]++;
    }

    std::vector<size_t> cumul(256, 0);
    size_t sum = 0;
    for (size_t i = 0; i < 256; ++i) {
        cumul[i] = sum;
        sum += frequency[i];
    }

    std::vector<byte> decoded(n);
    size_t j = originalIndex;
    for (size_t i = 0; i < n; ++i) {
        decoded[n - i - 1] = bwt[j];
        byte c = bwt[j];
        j = cumul[c] + occ[j];
    }

    return decoded;
}


std::vector<byte> readFileAsVector(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Ошибка открытия файла для чтения: " + filename);
    }
    std::vector<byte> data((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
    return data;
}

void writeVectorToFile(const std::string& filename, const std::vector<byte>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Ошибка открытия файла для записи: " + filename);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}


/*
//DEBUG
int main() {
    try {
        std::string inputFilename = "/Users/Egor 1/Desktop/Прога курсовик 2 курс/КУрсовая ООП/OOP_KursWork/Custom/enwik7.txt";
        std::vector<byte> input = readFileAsVector(inputFilename);

        auto start = std::chrono::high_resolution_clock::now();

        auto [encoded, originalIndex] = bwt_encode(input);
        std::vector<byte> decoded = bwt_decode(encoded, originalIndex);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::string encodedFilename = "/Users/Egor 1/Desktop/Прога курсовик 2 курс/КУрсовая ООП/OOP_KursWork/Custom/BTW.txt";
        writeVectorToFile(encodedFilename, encoded);

            std::string decodedFilename = "/Users/Egor 1/Desktop/Прога курсовик 2 курс/КУрсовая ООП/OOP_KursWork/Custom/outputcontrl.txt";
        writeVectorToFile(decodedFilename, decoded);

        if (input == decoded) {
            std::cout << "Декодирование выполнено успешно. Исходные и декодированные данные совпадают." << std::endl;
        } else {
            std::cout << "Ошибка: исходные и декодированные данные не совпадают." << std::endl;
        }

        std::cout << "Время выполнения: " << duration.count() << " секунд." << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
*/
