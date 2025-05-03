//
//  Quantizer.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef Quantizer_hpp
#define Quantizer_hpp


#include "QuantizationTables.hpp"  // Для доступа к DEFAULT_LUMA_QUANT_TABLE и DEFAULT_CHROMA_QUANT_TABLE
#include <vector>

// Функция квантования и де-квантования DCT-коэффициентов.
// Параметры:
// - dctCoeffs: матрица коэффициентов, полученных после прямого DCT;
// - quantTable: стандартная таблица квантования (8x8);
// - quality: уровень качества (1–100).
// Возвращает матрицу после квантования/де-квантования.
std::vector<std::vector<double>> applyQuantization(
    const std::vector<std::vector<double>>& dctCoeffs,
    const std::array<std::array<int, 8>, 8>& quantTable,
    int quality);



#endif /* Quantizer_hpp */
