//
//  ImageBlockProcessor.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

// ImageBlockProcessor.hpp
#ifndef IMAGE_BLOCK_PROCESSOR_HPP
#define IMAGE_BLOCK_PROCESSOR_HPP

#include "CommonTypes.hpp"

template<typename T>
ImageBlocks<T> splitImageIntoBlocks(const std::vector<T>& imageData,
                                  uint32_t width, uint32_t height,
                                  uint32_t blockSize,
                                  T paddingValue = T());

template<typename T>
std::vector<T> combineBlocksIntoImage(const ImageBlocks<T>& blocks,
                                    uint32_t originalWidth,
                                    uint32_t originalHeight);

// Явные инстанциации
extern template ImageBlocks<float> splitImageIntoBlocks<float>(
    const std::vector<float>&, uint32_t, uint32_t, uint32_t, float);
extern template std::vector<float> combineBlocksIntoImage<float>(
    const ImageBlocks<float>&, uint32_t, uint32_t);

#endif // IMAGE_BLOCK_PROCESSOR_HPP
