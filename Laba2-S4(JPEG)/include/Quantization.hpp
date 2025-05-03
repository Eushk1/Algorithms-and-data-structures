//
//  Quantization.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef QUANTIZATION_HPP
#define QUANTIZATION_HPP

#include "CommonTypes.hpp"
#include "QuantizationTables.hpp"
#include <vector>

// Уровень качества (1-100)
enum class QualityFactor {
    LOW = 50,
    MEDIUM = 75,
    HIGH = 90,
    MAX = 100
};

// Функция масштабирования матрицы квантования
std::array<std::array<float, 8>, 8> scaleQuantizationTable(
    const std::array<std::array<int, 8>, 8>& baseTable,
    QualityFactor quality);

// Квантование блока DCT коэффициентов
void quantizeBlock(std::vector<std::vector<float>>& dctBlock,
                 const std::array<std::array<float, 8>, 8>& quantTable);

// Обратное квантование блока
void dequantizeBlock(std::vector<std::vector<float>>& quantizedBlock,
                   const std::array<std::array<float, 8>, 8>& quantTable);

// Применение квантования ко всем блокам
void applyQuantization(ImageBlocks<float>& dctBlocks,
                     const std::array<std::array<float, 8>, 8>& lumaQuantTable,
                     const std::array<std::array<float, 8>, 8>& chromaQuantTable,
                     bool isLuma);

// Применение обратного квантования ко всем блокам
void applyDequantization(ImageBlocks<float>& quantizedBlocks,
                       const std::array<std::array<float, 8>, 8>& lumaQuantTable,
                       const std::array<std::array<float, 8>, 8>& chromaQuantTable,
                       bool isLuma);

#endif // QUANTIZATION_HPP
