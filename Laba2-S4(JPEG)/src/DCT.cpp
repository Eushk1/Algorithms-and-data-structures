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
#include <iostream>

std::vector<std::vector<double>> dct2d(const std::vector<std::vector<double>>& block) {
    size_t N = block.size();
    size_t M = block[0].size();
    std::vector<std::vector<double>> F(N, std::vector<double>(M, 0.0));
    
    // Precompute cosine matrices and coefficients
    std::vector<std::vector<double>> cos_u(N, std::vector<double>(N));
    std::vector<std::vector<double>> cos_v(M, std::vector<double>(M));
    
    for (size_t u = 0; u < N; u++) {
        for (size_t x = 0; x < N; x++) {
            cos_u[u][x] = std::cos(M_PI * (2 * x + 1) * u / (2.0 * N));
        }
    }
    
    for (size_t v = 0; v < M; v++) {
        for (size_t y = 0; y < M; y++) {
            cos_v[v][y] = std::cos(M_PI * (2 * y + 1) * v / (2.0 * M));
        }
    }
    
    // Compute DCT
    for (size_t u = 0; u < N; u++) {
        for (size_t v = 0; v < M; v++) {
            double sum = 0.0;
            for (size_t x = 0; x < N; x++) {
                double row_sum = 0.0;
                for (size_t y = 0; y < M; y++) {
                    row_sum += block[x][y] * cos_v[v][y];
                }
                sum += row_sum * cos_u[u][x];
            }
            
            double c_u = (u == 0) ? std::sqrt(1.0 / N) : std::sqrt(2.0 / N);
            double c_v = (v == 0) ? std::sqrt(1.0 / M) : std::sqrt(2.0 / M);
            F[u][v] = c_u * c_v * sum;
        }
    }
    
    return F;
}

std::vector<std::vector<double>> idct2d(const std::vector<std::vector<double>>& F) {
    size_t N = F.size();
    size_t M = F[0].size();
    std::vector<std::vector<double>> block(N, std::vector<double>(M, 0.0));
    
    // Precompute cosine matrices and coefficients
    std::vector<std::vector<double>> cos_x(N, std::vector<double>(N));
    std::vector<std::vector<double>> cos_y(M, std::vector<double>(M));
    
    for (size_t x = 0; x < N; x++) {
        for (size_t u = 0; u < N; u++) {
            cos_x[x][u] = std::cos(M_PI * (2 * x + 1) * u / (2.0 * N));
        }
    }
    
    for (size_t y = 0; y < M; y++) {
        for (size_t v = 0; v < M; v++) {
            cos_y[y][v] = std::cos(M_PI * (2 * y + 1) * v / (2.0 * M));
        }
    }
    
    // Compute IDCT
    for (size_t x = 0; x < N; x++) {
        for (size_t y = 0; y < M; y++) {
            double sum = 0.0;
            for (size_t u = 0; u < N; u++) {
                double row_sum = 0.0;
                for (size_t v = 0; v < M; v++) {
                    double c_u = (u == 0) ? std::sqrt(1.0 / N) : std::sqrt(2.0 / N);
                    double c_v = (v == 0) ? std::sqrt(1.0 / M) : std::sqrt(2.0 / M);
                    row_sum += c_u * c_v * F[u][v] * cos_y[y][v];
                }
                sum += row_sum * cos_x[x][u];
            }
            block[x][y] = sum;
        }
    }
    
    return block;
}

#endif // DCT_HPP
