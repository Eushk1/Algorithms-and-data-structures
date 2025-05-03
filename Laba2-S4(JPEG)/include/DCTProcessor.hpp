//
//  DCTProcessor.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef DCT_PROCESSOR_HPP
#define DCT_PROCESSOR_HPP

#include "CommonTypes.hpp"

constexpr int BLOCK_SIZE = 8;

// Преобразование одного блока
void forwardDCT(std::vector<std::vector<float>>& block);
void inverseDCT(std::vector<std::vector<float>>& block);

// Преобразование всех блоков
void applyDCTToBlocks(ImageBlocks<float>& blocks);
void applyIDCTToBlocks(ImageBlocks<float>& blocks);

#endif // DCT_PROCESSOR_HPP
