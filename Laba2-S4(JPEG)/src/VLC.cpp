//
//  VLC.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "VLC.hpp"
#include "HuffmanTables.hpp"
#include <bitset>

// DC-кодирование (разностное)
std::vector<uint8_t> VLC::encodeDC(const std::vector<int>& dc_deltas) {
    std::vector<uint8_t> encoded;
    for (int delta : dc_deltas) {
        // Определяем категорию величины (ITU-T81, стр. 88)
        int cat = (delta == 0) ? 0 : static_cast<int>(std::log2(std::abs(delta)) + 1;
        
        // Кодируем категорию и значение
        encoded.push_back(static_cast<uint8_t>(cat)); // Категория
        if (cat > 0) {
            encoded.push_back(static_cast<uint8_t>(delta & 0xFF)); // Младшие биты
        }
    }
    return encoded;
}

// DC-декодирование
std::vector<int> VLC::decodeDC(const std::vector<uint8_t>& encoded_data) {
    std::vector<int> dc_deltas;
    for (size_t i = 0; i < encoded_data.size(); ) {
        uint8_t cat = encoded_data[i++];
        int delta = 0;
        
        if (cat > 0) {
            delta = static_cast<int>(encoded_data[i++]);
            // Восстанавливаем полное значение
            if (delta < (1 << (cat - 1))) {
                delta -= (1 << cat) - 1;
            }
        }
        dc_deltas.push_back(delta);
    }
    return dc_deltas;
}

// AC-кодирование (RLE пары)
std::vector<uint8_t> VLC::encodeAC(const std::vector<std::pair<int, int>>& ac_rle) {
    std::vector<uint8_t> encoded;
    for (const auto& [run_length, value] : ac_rle) {
        // Кодируем run_length и категорию значения
        encoded.push_back(static_cast<uint8_t>(run_length));
        
        int cat = (value == 0) ? 0 : static_cast<int>(std::log2(std::abs(value))) + 1;
        encoded.push_back(static_cast<uint8_t>(cat));
        
        if (cat > 0) {
            encoded.push_back(static_cast<uint8_t>(value & 0xFF));
        }
    }
    return encoded;
}

// AC-декодирование
std::vector<std::pair<int, int>> VLC::decodeAC(const std::vector<uint8_t>& encoded_data) {
    std::vector<std::pair<int, int>> ac_rle;
    for (size_t i = 0; i < encoded_data.size(); ) {
        uint8_t run_length = encoded_data[i++];
        uint8_t cat = encoded_data[i++];
        int value = 0;
        
        if (cat > 0) {
            value = static_cast<int>(encoded_data[i++]);
            if (value < (1 << (cat - 1))) {
                value -= (1 << cat) - 1;
            }
        }
        ac_rle.emplace_back(run_length, value);
    }
    return ac_rle;
}
