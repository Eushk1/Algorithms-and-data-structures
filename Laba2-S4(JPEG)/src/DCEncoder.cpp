//
//  DCEncoder.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "DCEncoder.hpp"

namespace DCCoder {

    std::vector<int> encodeDC(const std::vector<int>& dcCoeffs) {
        std::vector<int> diff;
        if (dcCoeffs.empty()) {
            return diff;
        }
        // Первый элемент остается неизменным.
        diff.push_back(dcCoeffs[0]);
        // Для остальных элементов вычисляем разницу с предыдущим.
        for (size_t i = 1; i < dcCoeffs.size(); i++) {
            diff.push_back(dcCoeffs[i] - dcCoeffs[i - 1]);
        }
        return diff;
    }

    std::vector<int> decodeDC(const std::vector<int>& diffCoeffs) {
        std::vector<int> dc;
        if (diffCoeffs.empty()) {
            return dc;
        }
        // Первый элемент инициализируется напрямую.
        dc.push_back(diffCoeffs[0]);
        // Восстанавливаем по принципу: восстановленный[i] = восстановленный[i-1] + diff[i]
        for (size_t i = 1; i < diffCoeffs.size(); i++) {
            dc.push_back(dc[i - 1] + diffCoeffs[i]);
        }
        return dc;
    }

}


