//
//  RLE.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef RLE_hpp
#define RLE_hpp

#include <vector>
#include <utility>

namespace Zigzag {
    // Стандартный порядок зигзаг-сканирования JPEG
    static const int ORDER[64] = {
         0,  1,  5,  6, 14, 15, 27, 28,
         2,  4,  7, 13, 16, 26, 29, 42,
         3,  8, 12, 17, 25, 30, 41, 43,
         9, 11, 18, 24, 31, 40, 44, 53,
        10, 19, 23, 32, 39, 45, 52, 54,
        20, 22, 33, 38, 46, 51, 55, 60,
        21, 34, 37, 47, 50, 56, 59, 61,
        35, 36, 48, 49, 57, 58, 62, 63
    };

    // Преобразует блок 8×8 в вектор по зигзаг-порядку
    std::vector<int> scan(const std::vector<std::vector<int>>& block);
}

namespace RLE {
    // Кодирует AC-коэффициенты в пары (run_length, value)
    std::vector<std::pair<int, int>> encodeAC(const std::vector<int>& zigzag);

    // Декодирует AC-коэффициенты из RLE
    std::vector<int> decodeAC(const std::vector<std::pair<int, int>>& rle);
}

#endif /* RLE_hpp */
