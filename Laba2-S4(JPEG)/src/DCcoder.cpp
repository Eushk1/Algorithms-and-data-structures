//
//  DCcoder.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "DCcoder.hpp"


std::vector<int> DCCoder::compute_deltas(const std::vector<int>& dc_coeffs) {
    std::vector<int> deltas;
    deltas.reserve(dc_coeffs.size());
    deltas.push_back(dc_coeffs[0]); // Первый DC-коэффициент сохраняем как есть
    for (size_t i = 1; i < dc_coeffs.size(); ++i) {
        deltas.push_back(dc_coeffs[i] - dc_coeffs[i-1]); // Разности
    }
    return deltas;
}

std::vector<uint8_t> DCCoder::encode(const std::vector<int>& dc_coeffs) {
    const auto deltas = compute_deltas(dc_coeffs);
    return VLC::encode(deltas); // Используем готовый VLC-кодер
}

std::vector<int> DCCoder::decode(const std::vector<uint8_t>& encoded_data) {
    auto deltas = VLC::decode(encoded_data);
    std::vector<int> dc_coeffs;
    dc_coeffs.reserve(deltas.size());
    dc_coeffs.push_back(deltas[0]);
    for (size_t i = 1; i < deltas.size(); ++i) {
        dc_coeffs.push_back(dc_coeffs.back() + deltas[i]); // Восстановление DC
    }
    return dc_coeffs;
}
