//
//  Huffman.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef Huffman_hpp
#define Huffman_hpp

#include "Bitstream.hpp"
#include "HuffmanTables.hpp"
#include <vector>
#include <unordered_map>

class Huffman {
public:
    // Кодирование
    static std::vector<uint8_t> encodeDC(const std::vector<int>& dc_coeffs, bool is_luma = true);
    static std::vector<uint8_t> encodeAC(const std::vector<std::pair<int, int>>& ac_rle, bool is_luma = true);
    
    // Декодирование
    static std::vector<int> decodeDC(const std::vector<uint8_t>& encoded_data, bool is_luma = true);
    static std::vector<std::pair<int, int>> decodeAC(const std::vector<uint8_t>& encoded_data, bool is_luma = true);

private:
    // Вспомогательные структуры для декодирования
    struct HuffmanCode {
        uint8_t length;
        uint16_t code;
        int value;
    };
    
    static std::vector<HuffmanCode> buildDecodeTable(const std::unordered_map<int, std::pair<uint8_t, uint16_t>>& table);
};

#endif /* Huffman_hpp */
