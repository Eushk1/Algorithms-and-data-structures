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

class HuffmanEncoder {
public:
    static void encodeDC(int diff, bool isLuma, BitWriter& writer);
    static void encodeAC(int run, int value, bool isLuma, BitWriter& writer);
};

class HuffmanDecoder {
public:
    static int decodeDC(BitReader& reader, bool isLuma);
    static std::pair<int, int> decodeAC(BitReader& reader, bool isLuma);
};

#endif /* Huffman_hpp */
