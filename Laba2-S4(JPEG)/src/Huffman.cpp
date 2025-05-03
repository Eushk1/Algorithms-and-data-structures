//
//  Huffman.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "Huffman.hpp"
#include <bitset>

// Кодирование DC
std::vector<uint8_t> Huffman::encodeDC(const std::vector<int>& dc_coeffs, bool is_luma) {
    const auto& table = is_luma ? JPEGHuffmanTables::DC_LUM : JPEGHuffmanTables::DC_CHR;
    std::vector<uint8_t> encoded;
    
    for (int coeff : dc_coeffs) {
        int cat = (coeff == 0) ? 0 : static_cast<int>(std::log2(std::abs(coeff))) + 1;
        auto it = table.find(cat);
        if (it != table.end()) {
            // Записываем код Хаффмана
            uint16_t code = it->second.second;
            uint8_t length = it->second.first;
            
            // Упаковываем в битовый поток
            for (int i = length - 1; i >= 0; --i) {
                encoded.push_back((code >> i) & 1);
            }
            
            // Записываем дополнительные биты значения
            if (cat > 0) {
                int val = (coeff < 0) ? coeff + (1 << cat) - 1 : coeff;
                for (int i = cat - 1; i >= 0; --i) {
                    encoded.push_back((val >> i) & 1);
                }
            }
        }
    }
    return encoded;
}

// Декодирование DC
std::vector<int> Huffman::decodeDC(const std::vector<uint8_t>& encoded_data, bool is_luma) {
    const auto& table = is_luma ? JPEGHuffmanTables::DC_LUM : JPEGHuffmanTables::DC_CHR;
    auto decode_table = buildDecodeTable(table);
    std::vector<int> dc_coeffs;
    
    size_t bit_pos = 0;
    while (bit_pos < encoded_data.size()) {
        uint16_t current_code = 0;
        uint8_t current_length = 0;
        
        // Поиск совпадения в таблице
        HuffmanCode matched_code;
        bool found = false;
        
        for (const auto& hcode : decode_table) {
            if (bit_pos + hcode.length > encoded_data.size()) continue;
            
            current_code = 0;
            for (uint8_t i = 0; i < hcode.length; ++i) {
                current_code = (current_code << 1) | encoded_data[bit_pos + i];
            }
            
            if (current_code == hcode.code) {
                matched_code = hcode;
                found = true;
                break;
            }
        }
        
        if (!found) break;
        
        bit_pos += matched_code.length;
        int cat = matched_code.value;
        int coeff = 0;
        
        if (cat > 0) {
            if (bit_pos + cat > encoded_data.size()) break;
            
            // Чтение дополнительных битов
            int val = 0;
            for (int i = 0; i < cat; ++i) {
                val = (val << 1) | encoded_data[bit_pos + i];
            }
            bit_pos += cat;
            
            // Восстановление значения
            if (val < (1 << (cat - 1))) {
                val -= (1 << cat) - 1;
            }
            coeff = val;
        }
        
        dc_coeffs.push_back(coeff);
    }
    
    return dc_coeffs;
}

// Построение таблицы для декодирования
std::vector<Huffman::HuffmanCode> Huffman::buildDecodeTable(
    const std::unordered_map<int, std::pair<uint8_t, uint16_t>>& table) {
    std::vector<HuffmanCode> decode_table;
    
    for (const auto& [value, code_info] : table) {
        decode_table.push_back({
            code_info.first,  // length
            code_info.second, // code
            value            // value
        });
    }
    
    // Сортировка по длине кода для корректного декодирования
    std::sort(decode_table.begin(), decode_table.end(),
        [](const HuffmanCode& a, const HuffmanCode& b) {
            return a.length < b.length;
        });
    
    return decode_table;
}
