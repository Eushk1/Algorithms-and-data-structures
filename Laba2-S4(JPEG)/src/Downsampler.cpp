//
//  Downsampler.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

// Downsampler.cpp
#include "Downsampler.hpp"
#include <algorithm>

YCbCrImage downsampleYCbCr420(const YCbCrImage& input) {
    YCbCrImage output;
    output.width = input.width;
    output.height = input.height;
    
    // Канал Y оставляем без изменений
    output.Y = input.Y;
    
    // Даунсемплинг Cb и Cr с коэффициентом 2
    uint32_t newWidth = (input.width + 1) / 2;
    uint32_t newHeight = (input.height + 1) / 2;
    
    output.Cb.resize(newWidth * newHeight);
    output.Cr.resize(newWidth * newHeight);
    
    for (uint32_t y = 0; y < newHeight; y++) {
        for (uint32_t x = 0; x < newWidth; x++) {
            // Берем среднее значение из 4 пикселей (или меньше на границах)
            uint32_t srcX = x * 2;
            uint32_t srcY = y * 2;
            
            uint32_t count = 0;
            uint32_t sumCb = 0, sumCr = 0;
            
            for (uint32_t dy = 0; dy < 2 && (srcY + dy) < input.height; dy++) {
                for (uint32_t dx = 0; dx < 2 && (srcX + dx) < input.width; dx++) {
                    uint32_t idx = (srcY + dy) * input.width + (srcX + dx);
                    sumCb += input.Cb[idx];
                    sumCr += input.Cr[idx];
                    count++;
                }
            }
            
            uint32_t dstIdx = y * newWidth + x;
            output.Cb[dstIdx] = sumCb / count;
            output.Cr[dstIdx] = sumCr / count;
        }
    }
    
    return output;
}

YCbCrImage upsampleYCbCr420(const YCbCrImage& input) {
    YCbCrImage output;
    output.width = input.width;
    output.height = input.height;
    
    // Канал Y оставляем без изменений
    output.Y = input.Y;
    
    // Апсемплинг Cb и Cr с коэффициентом 2
    uint32_t origCbCrWidth = (input.width + 1) / 2;
    uint32_t origCbCrHeight = (input.height + 1) / 2;
    
    output.Cb.resize(input.width * input.height);
    output.Cr.resize(input.width * input.height);
    
    for (uint32_t y = 0; y < input.height; y++) {
        for (uint32_t x = 0; x < input.width; x++) {
            uint32_t srcX = x / 2;
            uint32_t srcY = y / 2;
            uint32_t srcIdx = srcY * origCbCrWidth + srcX;
            
            uint32_t dstIdx = y * input.width + x;
            output.Cb[dstIdx] = input.Cb[srcIdx];
            output.Cr[dstIdx] = input.Cr[srcIdx];
        }
    }
    
    return output;
}
