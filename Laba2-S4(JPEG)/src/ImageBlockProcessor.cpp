//
//  ImageBlockProcessor.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "ImageBlockProcessor.hpp"
#include <vector>
#include <algorithm>

template<typename T>
ImageBlocks<T> splitImageIntoBlocks(const std::vector<T>& imageData,
                                 uint32_t width, uint32_t height,
                                 uint32_t blockSize,
                                 T paddingValue) {
    // Вычисляем количество блоков по горизонтали и вертикали
    uint32_t blocksX = (width + blockSize - 1) / blockSize;
    uint32_t blocksY = (height + blockSize - 1) / blockSize;
    
    ImageBlocks<T> result(blocksY, std::vector<ImageBlock<T>>(blocksX));
    
    for (uint32_t by = 0; by < blocksY; by++) {
        for (uint32_t bx = 0; bx < blocksX; bx++) {
            // Координаты начала блока в исходном изображении
            uint32_t startX = bx * blockSize;
            uint32_t startY = by * blockSize;
            
            // Создаем блок
            ImageBlock<T> block;
            block.originalX = startX;
            block.originalY = startY;
            block.data.resize(blockSize, std::vector<T>(blockSize));
            
            // Заполняем блок данными
            for (uint32_t y = 0; y < blockSize; y++) {
                for (uint32_t x = 0; x < blockSize; x++) {
                    uint32_t imgX = startX + x;
                    uint32_t imgY = startY + y;
                    
                    if (imgX < width && imgY < height) {
                        // Данные из изображения
                        block.data[y][x] = imageData[imgY * width + imgX];
                    } else {
                        // Заполнение паддингом
                        block.data[y][x] = paddingValue;
                        block.isPadding = true;
                    }
                }
            }
            
            result[by][bx] = block;
        }
    }
    
    return result;
}

template<typename T>
std::vector<T> combineBlocksIntoImage(const ImageBlocks<T>& blocks,
                                    uint32_t originalWidth,
                                    uint32_t originalHeight) {
    if (blocks.empty() || blocks[0].empty()) {
        return {};
    }
    
    uint32_t blockSize = blocks[0][0].data.size();
    std::vector<T> result(originalWidth * originalHeight);
    
    for (const auto& row : blocks) {
        for (const auto& block : row) {
            for (uint32_t y = 0; y < blockSize; y++) {
                for (uint32_t x = 0; x < blockSize; x++) {
                    uint32_t imgX = block.originalX + x;
                    uint32_t imgY = block.originalY + y;
                    
                    if (imgX < originalWidth && imgY < originalHeight) {
                        result[imgY * originalWidth + imgX] = block.data[y][x];
                    }
                }
            }
        }
    }
    
    return result;
}

// Явные инстанциации шаблонов для поддерживаемых типов
template ImageBlocks<float> splitImageIntoBlocks<float>(
    const std::vector<float>&, uint32_t, uint32_t, uint32_t, float);
template std::vector<float> combineBlocksIntoImage<float>(
    const ImageBlocks<float>&, uint32_t, uint32_t);
