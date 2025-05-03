//
//  ColorSpaceConverter.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

// ColorSpaceConverter.hpp
#ifndef COLOR_SPACE_CONVERTER_HPP
#define COLOR_SPACE_CONVERTER_HPP

#include "ImageFileIO.hpp"
#include <vector>

std::vector<YCbCrPixel> convertRGBToYCbCr(const std::vector<RGBPixel>& rgbPixels);
std::vector<RGBPixel> convertYCbCrToRGB(const std::vector<YCbCrPixel>& ycbcrPixels);

#endif // COLOR_SPACE_CONVERTER_HPP
