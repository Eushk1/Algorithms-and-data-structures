//
//  Downsampler.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef Downsampler_hpp
#define Downsampler_hpp

#include "ImageFileIO.hpp"
#include <vector>


YCbCrImage downsampleYCbCr420(const YCbCrImage& input);
YCbCrImage upsampleYCbCr420(const YCbCrImage& input);

#endif /* Downsampler_hpp */
