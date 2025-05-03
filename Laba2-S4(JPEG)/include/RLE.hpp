//
//  RLE.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef RLE_hpp
#define RLE_hpp

#include <vector>
#include <utility>

using RLEPair = std::pair<uint8_t, int16_t>;

// Энкодер
std::vector<RLEPair> encodeRLEAC(const std::vector<int16_t>& acCoeffs);

// Декодер
std::vector<int16_t> decodeRLEAC(const std::vector<RLEPair>& rleData);

#endif /* RLE_hpp */
