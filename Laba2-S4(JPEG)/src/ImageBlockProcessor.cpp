#include "ImageBlockProcessor.hpp"
#include <algorithm>

template<typename T>
ImageBlocks<T> ImageBlockProcessor::splitToBlocks(const std::vector<T>& imageData,
                                                uint32_t width, uint32_t height,
                                                uint32_t blockSize,
                                                const T& paddingValue) {
    ImageBlocks<T> blocks;
    
    uint32_t blocksX = (width + blockSize - 1) / blockSize;
    uint32_t blocksY = (height + blockSize - 1) / blockSize;
    
    blocks.resize(blocksY, std::vector<ImageBlock<T>>(blocksX));
    
    for (uint32_t by = 0; by < blocksY; by++) {
        for (uint32_t bx = 0; bx < blocksX; bx++) {
            ImageBlock<T> block;
            block.originalX = bx * blockSize;
            block.originalY = by * blockSize;
            block.data.resize(blockSize, std::vector<T>(blockSize));
            
            for (uint32_t y = 0; y < blockSize; y++) {
                for (uint32_t x = 0; x < blockSize; x++) {
                    uint32_t imgX = block.originalX + x;
                    uint32_t imgY = block.originalY + y;
                    
                    if (imgX < width && imgY < height) {
                        block.data[y][x] = imageData[imgY * width + imgX];
                    } else {
                        block.data[y][x] = paddingValue;
                        block.isPadding = true;
                    }
                }
            }
            
            blocks[by][bx] = block;
        }
    }
    
    return blocks;
}

template<typename T>
std::vector<T> ImageBlockProcessor::combineBlocks(const ImageBlocks<T>& blocks,
                                                uint32_t originalWidth,
                                                uint32_t originalHeight) {
    if (blocks.empty() || blocks[0].empty()) {
        return {};
    }
    
    uint32_t blockSize = blocks[0][0].data.size();
    std::vector<T> imageData(originalWidth * originalHeight);
    
    for (const auto& row : blocks) {
        for (const auto& block : row) {
            for (uint32_t y = 0; y < blockSize; y++) {
                for (uint32_t x = 0; x < blockSize; x++) {
                    uint32_t imgX = block.originalX + x;
                    uint32_t imgY = block.originalY + y;
                    
                    if (imgX < originalWidth && imgY < originalHeight) {
                        imageData[imgY * originalWidth + imgX] = block.data[y][x];
                    }
                }
            }
        }
    }
    
    return imageData;
}



template ImageBlocks<uint8_t> ImageBlockProcessor::splitToBlocks<uint8_t>(
    const std::vector<uint8_t>&, uint32_t, uint32_t, uint32_t, const uint8_t&);

template std::vector<uint8_t> ImageBlockProcessor::combineBlocks<uint8_t>(
    const ImageBlocks<uint8_t>&, uint32_t, uint32_t);
