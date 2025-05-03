//
//  Quantization.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "Quantization.hpp"
#include <cmath>

std::array<std::array<float, 8>, 8> scaleQuantizationTable(
    const std::array<std::array<int, 8>, 8>& baseTable,
    QualityFactor quality) {
    
    std::array<std::array<float, 8>, 8> scaledTable;
    float scaleFactor;
    
    // Вычисляем масштабирующий коэффициент
    if (quality == QualityFactor::MAX) {
        scaleFactor = 1.0f; // Максимальное качество - минимальное квантование
    } else {
        int q = static_cast<int>(quality);
        scaleFactor = q < 50 ? 5000.0f / q : 200.0f - 2 * q;
        scaleFactor /= 100.0f;
    }
    
    // Масштабируем таблицу
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            float value = baseTable[i][j] * scaleFactor;
            scaledTable[i][j] = std::max(1.0f, std::min(value, 255.0f));
        }
    }
    
    return scaledTable;
}

void quantizeBlock(std::vector<std::vector<float>>& dctBlock,
                 const std::array<std::array<float, 8>, 8>& quantTable) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            dctBlock[i][j] = std::round(dctBlock[i][j] / quantTable[i][j]);
        }
    }
}

void dequantizeBlock(std::vector<std::vector<float>>& quantizedBlock,
                   const std::array<std::array<float, 8>, 8>& quantTable) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            quantizedBlock[i][j] *= quantTable[i][j];
        }
    }
}

void applyQuantization(ImageBlocks<float>& dctBlocks,
                     const std::array<std::array<float, 8>, 8>& lumaQuantTable,
                     const std::array<std::array<float, 8>, 8>& chromaQuantTable,
                     bool isLuma) {
    const auto& quantTable = isLuma ? lumaQuantTable : chromaQuantTable;
    
    for (auto& row : dctBlocks) {
        for (auto& block : row) {
            quantizeBlock(block.data, quantTable);
        }
    }
}

void applyDequantization(ImageBlocks<float>& quantizedBlocks,
                       const std::array<std::array<float, 8>, 8>& lumaQuantTable,
                       const std::array<std::array<float, 8>, 8>& chromaQuantTable,
                       bool isLuma) {
    const auto& quantTable = isLuma ? lumaQuantTable : chromaQuantTable;
    
    for (auto& row : quantizedBlocks) {
        for (auto& block : row) {
            dequantizeBlock(block.data, quantTable);
        }
    }
}
