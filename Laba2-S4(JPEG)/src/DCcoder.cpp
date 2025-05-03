//
//  DCcoder.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "DCcoder.hpp"

std::vector<int> encodeDCDifferences(const std::vector<int>& dcCoeffs) {
    std::vector<int> diffs(dcCoeffs.size());
    diffs[0] = dcCoeffs[0];
    for (size_t i = 1; i < dcCoeffs.size(); ++i) {
        diffs[i] = dcCoeffs[i] - dcCoeffs[i-1];
    }
    return diffs;
}

std::vector<int> decodeDCDifferences(const std::vector<int>& diffs) {
    std::vector<int> dcCoeffs(diffs.size());
    dcCoeffs[0] = diffs[0];
    for (size_t i = 1; i < diffs.size(); ++i) {
        dcCoeffs[i] = dcCoeffs[i-1] + diffs[i];
    }
    return dcCoeffs;
}
