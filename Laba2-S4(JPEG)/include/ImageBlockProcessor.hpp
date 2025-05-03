//
//  ImageBlockProcessor.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef IMAGE_BLOCK_PROCESSOR_HPP
#define IMAGE_BLOCK_PROCESSOR_HPP

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

class ImageBlockProcessor {
public:
    template<typename T>
    static ImageBlocks<T> splitToBlocks(const std::vector<T>& imageData,
                                     uint32_t width, uint32_t height,
                                     uint32_t blockSize = 8,
                                     const T& paddingValue = T());

    template<typename T>
    static std::vector<T> combineBlocks(const ImageBlocks<T>& blocks,
                                     uint32_t originalWidth,
                                     uint32_t originalHeight);
    
    // Переносим реализацию processBlocks в заголовочный файл
    template<typename T, typename Func>
    static void processBlocks(ImageBlocks<T>& blocks, Func func) {
        for (auto& row : blocks) {
            for (auto& block : row) {
                func(block);
            }
        }
    }
};

// Явные инстанциации для splitToBlocks и combineBlocks
extern template ImageBlocks<uint8_t> ImageBlockProcessor::splitToBlocks<uint8_t>(
    const std::vector<uint8_t>&, uint32_t, uint32_t, uint32_t, const uint8_t&);

extern template std::vector<uint8_t> ImageBlockProcessor::combineBlocks<uint8_t>(
    const ImageBlocks<uint8_t>&, uint32_t, uint32_t);

#endif // IMAGE_BLOCK_PROCESSOR_HPP
