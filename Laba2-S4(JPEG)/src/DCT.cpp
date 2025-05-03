//
//  DCT.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

// DCT.cpp
#ifndef DCT_HPP
#define DCT_HPP
#include "DCT.hpp"
#include <cmath>
#include <vector>

// Реализация 2D DCT-II
std::vector<std::vector<double>> dct2d(const std::vector<std::vector<double>>& block) {
    size_t N = block.size();
    size_t M = block[0].size();
    std::vector<std::vector<double>> F(N, std::vector<double>(M, 0.0));

    for (size_t u = 0; u < N; u++) {
        for (size_t v = 0; v < M; v++) {
            double sum = 0.0;
            for (size_t x = 0; x < N; x++) {
                for (size_t y = 0; y < M; y++) {
                    sum += block[x][y] *
                           std::cos(M_PI * (2 * x + 1) * u / (2.0 * N)) *
                           std::cos(M_PI * (2 * y + 1) * v / (2.0 * M));
                }
            }
            // Коэффициенты нормализации
            double c_u = (u == 0) ? std::sqrt(1.0 / N) : std::sqrt(2.0 / N);
            double c_v = (v == 0) ? std::sqrt(1.0 / M) : std::sqrt(2.0 / M);
            F[u][v] = c_u * c_v * sum;
        }
    }
    return F;
}

// Реализация обратного 2D DCT-II (IDCT-II)
std::vector<std::vector<double>> idct2d(const std::vector<std::vector<double>>& F) {
    size_t N = F.size();
    size_t M = F[0].size();
    std::vector<std::vector<double>> block(N, std::vector<double>(M, 0.0));

    for (size_t x = 0; x < N; x++) {
        for (size_t y = 0; y < M; y++) {
            double sum = 0.0;
            for (size_t u = 0; u < N; u++) {
                for (size_t v = 0; v < M; v++) {
                    double c_u = (u == 0) ? std::sqrt(1.0 / N) : std::sqrt(2.0 / N);
                    double c_v = (v == 0) ? std::sqrt(1.0 / M) : std::sqrt(2.0 / M);
                    sum += c_u * c_v * F[u][v] *
                           std::cos(M_PI * (2 * x + 1) * u / (2.0 * N)) *
                           std::cos(M_PI * (2 * y + 1) * v / (2.0 * M));
                }
            }
            block[x][y] = sum;
        }
    }
    return block;
}

#endif // DCT_HPP
