//
//  Quantizer.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef Quantizer_hpp
#define Quantizer_hpp


#include "QuantizationTables.hpp"
#include <vector>


std::vector<std::vector<double>> applyQuantization(
    const std::vector<std::vector<double>>& dctCoeffs,
    const std::array<std::array<int, 8>, 8>& quantTable,
    int quality);



#endif /* Quantizer_hpp */
