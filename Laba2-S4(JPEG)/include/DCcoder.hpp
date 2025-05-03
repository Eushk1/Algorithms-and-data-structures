//
//  DCcoder.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef DCcoder_hpp
#define DCcoder_hpp

#include <vector>

// Энкодер
std::vector<int> encodeDCDifferences(const std::vector<int>& dcCoeffs);

// Декодер
std::vector<int> decodeDCDifferences(const std::vector<int>& diffs);
#endif /* DCcoder_hpp */
