//
//  ColorSpaceConverter.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef ColorSpaceConverter_hpp
#define ColorSpaceConverter_hpp


#include "ColorSpaceConverter.hpp"
#include <cmath>

std::vector<YCbCrPixel> convertRGBToYCbCr(const std::vector<RGBPixel>& rgbPixels) {
    std::vector<YCbCrPixel> result(rgbPixels.size());
    
    for (size_t i = 0; i < rgbPixels.size(); i++) {
        const auto& rgb = rgbPixels[i];
        auto& ycbcr = result[i];
        

        ycbcr.Y  = static_cast<uint8_t>( 0.299 * rgb.R + 0.587 * rgb.G + 0.114 * rgb.B);
        ycbcr.Cb = static_cast<uint8_t>(-0.1687 * rgb.R - 0.3313 * rgb.G + 0.5 * rgb.B + 128);
        ycbcr.Cr = static_cast<uint8_t>( 0.5 * rgb.R - 0.4187 * rgb.G - 0.0813 * rgb.B + 128);
    }
    
    return result;
}

std::vector<RGBPixel> convertYCbCrToRGB(const std::vector<YCbCrPixel>& ycbcrPixels) {
    std::vector<RGBPixel> result(ycbcrPixels.size());
    
    for (size_t i = 0; i < ycbcrPixels.size(); i++) {
        const auto& ycbcr = ycbcrPixels[i];
        auto& rgb = result[i];
        

        double Y = ycbcr.Y;
        double Cb = ycbcr.Cb - 128;
        double Cr = ycbcr.Cr - 128;
        
        rgb.R = static_cast<uint8_t>(std::clamp(Y + 1.402 * Cr, 0.0, 255.0));
        rgb.G = static_cast<uint8_t>(std::clamp(Y - 0.34414 * Cb - 0.71414 * Cr, 0.0, 255.0));
        rgb.B = static_cast<uint8_t>(std::clamp(Y + 1.772 * Cb, 0.0, 255.0));
    }
    
    return result;
}
#endif /* ColorSpaceConverter_hpp */
