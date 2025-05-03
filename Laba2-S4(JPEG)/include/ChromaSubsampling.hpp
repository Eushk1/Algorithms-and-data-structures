//
//  ChromaSubsampling.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef ChromaSubsampling_hpp
#define ChromaSubsampling_hpp


#include "ImageFileIO.hpp"
#include <vector>

namespace ChromaSubsampling {

// Даунсэмплинг одного канала
std::vector<uint8_t> downsampleChannel(const std::vector<uint8_t>& channel,
                                     uint32_t width, uint32_t height,
                                     uint32_t newWidth, uint32_t newHeight);

// Апсэмплинг одного канала
std::vector<uint8_t> upsampleChannel(const std::vector<uint8_t>& downsampled,
                                   uint32_t originalWidth, uint32_t originalHeight,
                                   uint32_t downsampledWidth, uint32_t downsampledHeight);

// Даунсэмплинг изображения в формат 4:2:0
YCbCrImage downsample420(const YCbCrImage& image);

// Апсэмплинг изображения из формата 4:2:0
YCbCrImage upsample420(const YCbCrImage& downsampled);

} // namespace ChromaSubsampling

#endif /* ChromaSubsampling_hpp */
