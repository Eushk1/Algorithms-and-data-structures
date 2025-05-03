//
//  RLE.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "RLE.hpp"
#include <stdexcept>

std::vector<int> Zigzag::scan(const std::vector<std::vector<int>>& block) {
    if (block.size() != 8 || block[0].size() != 8) {
        throw std::invalid_argument("Block must be 8x8 for zigzag scan");
    }

    std::vector<int> zigzag(64);
    for (int i = 0; i < 64; ++i) {
        int row = ORDER[i] / 8;
        int col = ORDER[i] % 8;
        zigzag[i] = block[row][col];
    }
    return zigzag;
}

std::vector<std::pair<int, int>> RLE::encodeAC(const std::vector<int>& zigzag) {
    std::vector<std::pair<int, int>> rle;
    int zero_count = 0;

    for (size_t i = 1; i < zigzag.size(); ++i) { // Пропускаем DC (индекс 0)
        if (zigzag[i] == 0) {
            zero_count++;
        } else {
            while (zero_count >= 16) { // JPEG ограничение: run_length <= 15
                rle.emplace_back(15, 0); // Маркер ZRL (16 нулей)
                zero_count -= 16;
            }
            rle.emplace_back(zero_count, zigzag[i]);
            zero_count = 0;
        }
    }

    if (zero_count > 0 || rle.empty()) {
        rle.emplace_back(0, 0); // EOB (End of Block)
    }
    return rle;
}

std::vector<int> RLE::decodeAC(const std::vector<std::pair<int, int>>& rle) {
    std::vector<int> zigzag(64, 0);
    size_t pos = 1; // Начинаем с первого AC-коэффициента (DC уже обработан)

    for (const auto& [run, value] : rle) {
        if (run == 0 && value == 0) { // EOB
            break;
        }
        while (run-- > 0 && pos < 63) {
            zigzag[pos++] = 0;
        }
        if (pos < 64) {
            zigzag[pos++] = value;
        }
    }
    return zigzag;
}
