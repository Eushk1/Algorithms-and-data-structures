//
//  DCT.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef DCT_hpp
#define DCT_hpp


#include <vector>

// Функция для выполнения 2D DCT-II преобразования
std::vector<std::vector<double>> dct2d(const std::vector<std::vector<double>>& block);

// Функция для выполнения обратного 2D DCT-II преобразования (IDCT-II)
std::vector<std::vector<double>> idct2d(const std::vector<std::vector<double>>& dctBlock);

#endif /* DCT_hpp */
