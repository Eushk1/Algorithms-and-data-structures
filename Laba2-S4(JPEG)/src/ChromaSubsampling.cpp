//
//  ChromaSubsampling.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "ChromaSubsampling.hpp"
#include <algorithm>
#include <cmath>

namespace ChromaSubsampling {

std::vector<uint8_t> downsampleChannel(const std::vector<uint8_t>& channel,
                                     uint32_t width, uint32_t height,
                                     uint32_t newWidth, uint32_t newHeight) {
    std::vector<uint8_t> downsampled(newWidth * newHeight);
    
    float scaleX = static_cast<float>(width) / newWidth;
    float scaleY = static_cast<float>(height) / newHeight;
    
    for (uint32_t y = 0; y < newHeight; y++) {
        for (uint32_t x = 0; x < newWidth; x++) {

            float centerX = (x + 0.5f) * scaleX;
            float centerY = (y + 0.5f) * scaleY;
            

            uint32_t x1 = static_cast<uint32_t>(centerX - 0.5f);
            uint32_t y1 = static_cast<uint32_t>(centerY - 0.5f);
            uint32_t x2 = std::min(x1 + 1, width - 1);
            uint32_t y2 = std::min(y1 + 1, height - 1);
            

            uint32_t sum = channel[y1 * width + x1] +
                          channel[y1 * width + x2] +
                          channel[y2 * width + x1] +
                          channel[y2 * width + x2];
            
            downsampled[y * newWidth + x] = static_cast<uint8_t>(sum / 4);
        }
    }
    
    return downsampled;
}

std::vector<uint8_t> upsampleChannel(const std::vector<uint8_t>& downsampled,
                                   uint32_t originalWidth, uint32_t originalHeight,
                                   uint32_t downsampledWidth, uint32_t downsampledHeight) {
    std::vector<uint8_t> upsampled(originalWidth * originalHeight);
    
    float scaleX = static_cast<float>(downsampledWidth) / originalWidth;
    float scaleY = static_cast<float>(downsampledHeight) / originalHeight;
    
    for (uint32_t y = 0; y < originalHeight; y++) {
        for (uint32_t x = 0; x < originalWidth; x++) {
            float srcX = x * scaleX;
            float srcY = y * scaleY;
            
            uint32_t x1 = static_cast<uint32_t>(srcX);
            uint32_t y1 = static_cast<uint32_t>(srcY);
            uint32_t x2 = std::min(x1 + 1, downsampledWidth - 1);
            uint32_t y2 = std::min(y1 + 1, downsampledHeight - 1);
            
            float dx = srcX - x1;
            float dy = srcY - y1;
            

            float v1 = downsampled[y1 * downsampledWidth + x1];
            float v2 = downsampled[y1 * downsampledWidth + x2];
            float v3 = downsampled[y2 * downsampledWidth + x1];
            float v4 = downsampled[y2 * downsampledWidth + x2];
            
            float val = v1 * (1 - dx) * (1 - dy) +
                       v2 * dx * (1 - dy) +
                       v3 * (1 - dx) * dy +
                       v4 * dx * dy;
            
            upsampled[y * originalWidth + x] = static_cast<uint8_t>(std::round(val));
        }
    }
    
    return upsampled;
}

YCbCrImage downsample420(const YCbCrImage& image) {
    YCbCrImage downsampled;
    downsampled.width = image.width;
    downsampled.height = image.height;
    downsampled.Y = image.Y;
    
    uint32_t newWidth = (image.width + 1) / 2;
    uint32_t newHeight = (image.height + 1) / 2;
    
    downsampled.Cb = downsampleChannel(image.Cb, image.width, image.height, newWidth, newHeight);
    downsampled.Cr = downsampleChannel(image.Cr, image.width, image.height, newWidth, newHeight);
    
    return downsampled;
}

YCbCrImage upsample420(const YCbCrImage& downsampled) {
    YCbCrImage image;
    image.width = downsampled.width;
    image.height = downsampled.height;
    image.Y = downsampled.Y;
    
    uint32_t downWidth = (downsampled.width + 1) / 2;
    uint32_t downHeight = (downsampled.height + 1) / 2;
    
    image.Cb = upsampleChannel(downsampled.Cb, downsampled.width, downsampled.height, downWidth, downHeight);
    image.Cr = upsampleChannel(downsampled.Cr, downsampled.width, downsampled.height, downWidth, downHeight);
    
    return image;
}

} // namespace ChromaSubsampling
