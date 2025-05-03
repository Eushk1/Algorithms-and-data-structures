//
//  DCTProcessor.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "DCTProcessor.hpp"
#include <cmath>

namespace {
    const float PI = 3.14159265358979323846f;
    const float INV_SQRT_2 = 0.70710678118654752440f;
    
    float alpha(int u) {
        return u == 0 ? INV_SQRT_2 : 1.0f;
    }
}

void forwardDCT(std::vector<std::vector<float>>& block) {
    std::vector<std::vector<float>> temp(BLOCK_SIZE, std::vector<float>(BLOCK_SIZE));
    
    for (int y = 0; y < BLOCK_SIZE; y++) {
        for (int u = 0; u < BLOCK_SIZE; u++) {
            float sum = 0.0f;
            for (int x = 0; x < BLOCK_SIZE; x++) {
                sum += block[y][x] * cos((2*x+1)*u*PI/16);
            }
            temp[y][u] = 0.5f * alpha(u) * sum;
        }
    }
    
    for (int u = 0; u < BLOCK_SIZE; u++) {
        for (int v = 0; v < BLOCK_SIZE; v++) {
            float sum = 0.0f;
            for (int y = 0; y < BLOCK_SIZE; y++) {
                sum += temp[y][u] * cos((2*y+1)*v*PI/16);
            }
            block[u][v] = 0.5f * alpha(v) * sum;
        }
    }
}

void inverseDCT(std::vector<std::vector<float>>& block) {
    std::vector<std::vector<float>> temp(BLOCK_SIZE, std::vector<float>(BLOCK_SIZE));
    
    for (int y = 0; y < BLOCK_SIZE; y++) {
        for (int x = 0; x < BLOCK_SIZE; x++) {
            float sum = 0.0f;
            for (int u = 0; u < BLOCK_SIZE; u++) {
                sum += alpha(u) * block[y][u] * cos((2*x+1)*u*PI/16);
            }
            temp[y][x] = 0.5f * sum;
        }
    }
    
    for (int x = 0; x < BLOCK_SIZE; x++) {
        for (int y = 0; y < BLOCK_SIZE; y++) {
            float sum = 0.0f;
            for (int v = 0; v < BLOCK_SIZE; v++) {
                sum += alpha(v) * temp[v][x] * cos((2*y+1)*v*PI/16);
            }
            block[y][x] = 0.5f * sum;
        }
    }
}

void applyDCTToBlocks(ImageBlocks<float>& blocks) {
    for (auto& row : blocks) {
        for (auto& block : row) {
            forwardDCT(block.data);
        }
    }
}

void applyIDCTToBlocks(ImageBlocks<float>& blocks) {
    for (auto& row : blocks) {
        for (auto& block : row) {
            inverseDCT(block.data);
        }
    }
}
