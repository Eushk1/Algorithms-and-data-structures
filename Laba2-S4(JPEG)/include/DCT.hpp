//
//  DCT.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef DCT_hpp
#define DCT_hpp


#include <vector>


std::vector<std::vector<double>> dct2d(const std::vector<std::vector<double>>& block);


std::vector<std::vector<double>> idct2d(const std::vector<std::vector<double>>& dctBlock);

#endif /* DCT_hpp */
