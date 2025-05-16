//
//  ZigZag.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef ZIGZAG_HPP
#define ZIGZAG_HPP

#include <array>

namespace JPEGEntropy {

class ZigZag {
public:

    template <typename T>
    static std::array<T, 64> reorder(const std::array<T, 64>& block) {
        static const std::array<int, 64> order = {{
            0, 1, 5, 6, 14, 15, 27, 28,
            2, 4, 7, 13, 16, 26, 29, 42,
            3, 8, 12, 17, 25, 30, 41, 43,
            9, 11, 18, 24, 31, 40, 44, 53,
            10, 19, 23, 32, 39, 45, 52, 54,
            20, 22, 33, 38, 46, 51, 55, 57,
            21, 34, 37, 47, 50, 56, 58, 59,
            35, 36, 48, 49, 60, 61, 62, 63
        }};
        std::array<T, 64> output{};
        for (size_t i = 0; i < order.size(); i++) {
            output[i] = block[order[i]];
        }
        return output;
    }
};

} // namespace JPEGEntropy

#endif // ZIGZAG_HPP


