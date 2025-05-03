//
//  CommonTypes.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#include <vector>
#include <cstdint>

template<typename T>
struct ImageBlock {
    std::vector<std::vector<T>> data;
    uint32_t originalX;
    uint32_t originalY;
    bool isPadding = false;
};

template<typename T>
using ImageBlocks = std::vector<std::vector<ImageBlock<T>>>;

#endif
