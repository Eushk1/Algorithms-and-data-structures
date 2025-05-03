//
//  RLE.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "RLE.hpp"

std::vector<RLEPair> encodeRLEAC(const std::vector<int16_t>& acCoeffs) {
    std::vector<RLEPair> encoded;
    int zeroRun = 0;
    for (const auto& coeff : acCoeffs) {
        if (coeff == 0) {
            zeroRun++;
        } else {
            while (zeroRun >= 16) {
                encoded.emplace_back(15, 0); // ZRL
                zeroRun -= 16;
            }
            encoded.emplace_back(zeroRun, coeff);
            zeroRun = 0;
        }
    }
    encoded.emplace_back(0, 0); // EOB
    return encoded;
}

std::vector<int16_t> decodeRLEAC(const std::vector<RLEPair>& rleData) {
    std::vector<int16_t> acCoeffs;
    for (const auto& [run, value] : rleData) {
        if (run == 0 && value == 0) break; // EOB
        acCoeffs.insert(acCoeffs.end(), run, 0);
        acCoeffs.push_back(value);
    }
    acCoeffs.resize(63, 0); // Дополняем до 63 элементов
    return acCoeffs;
}
