//
//  Huffman.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef Huffman_hpp

#include "Bitstream.hpp"
#include "HuffmanTables.hpp"
#include <vector>
#include <utility>

class Huffman {
public:
    // Кодирование
    static void encodeDC(BitWriter& writer, int value, bool isLuma);
    static void encodeAC(BitWriter& writer, const std::vector<std::pair<int, int>>& acCoeffs, bool isLuma);
    
    // Декодирование
    static int decodeDC(BitReader& reader, bool isLuma);
    static std::vector<std::pair<int, int>> decodeAC(BitReader& reader, bool isLuma);
    
private:
    static int getCategory(int value);
    static int readValue(BitReader& reader, int category);
    
    template<typename T>
    static T decodeValue(BitReader& reader, const std::unordered_map<T, std::pair<uint8_t, uint16_t>>& table);
};

#endif /* Huffman_hpp */
