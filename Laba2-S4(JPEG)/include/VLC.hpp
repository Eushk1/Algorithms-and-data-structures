//
//  VLC.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef VLC_hpp
#define VLC_hpp

#include <vector>
#include <cstdint>

// Энкодер
void encodeVLC(int value, bool isDC, bool isLuma, BitWriter& writer);

// Декодер
int decodeVLC(BitReader& reader, bool isDC, bool isLuma);
#endif /* VLC_hpp */
