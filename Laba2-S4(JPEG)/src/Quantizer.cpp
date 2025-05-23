//
//  Quantizer.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "Quantizer.hpp"
#include <cmath>


static double getScalingFactor(int quality) {
    if (quality < 1) quality = 1;
    if (quality > 100) quality = 100;
    if (quality < 50)
        return 5000.0 / quality;
    else
        return 200 - quality * 2;
}

std::vector<std::vector<double>> applyQuantization(
    const std::vector<std::vector<double>>& dctCoeffs,
    const std::array<std::array<int, 8>, 8>& quantTable,
    int quality)
{
    double scale = getScalingFactor(quality);


    std::vector<std::vector<double>> quantized(dctCoeffs.size(), std::vector<double>(dctCoeffs[0].size(), 0.0));


    for (size_t i = 0; i < dctCoeffs.size(); i++) {
        for (size_t j = 0; j < dctCoeffs[i].size(); j++) {
            double q = 1.0;
            if (i < quantTable.size() && j < quantTable[i].size()) {
                int tableVal = quantTable[i][j];

                q = static_cast<double>((tableVal * scale + 50) / 100);
                if (q < 1.0)
                    q = 1.0;
            }

            double quantVal = std::round(dctCoeffs[i][j] / q);
            quantized[i][j] = quantVal * q;
        }
    }
    return quantized;
}
