//
//  VLC.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "VLC.hpp"
#include "Huffman/HuffmanTables.hpp"

void encodeVLC(int value, bool isDC, bool isLuma, BitWriter& writer) {
    int size = (value == 0) ? 0 : (int)std::log2(abs(value)) + 1;
    auto& table = isDC ? (isLuma ? HUFFMAN_DC_LUMA : HUFFMAN_DC_CHROMA)
                       : (isLuma ? HUFFMAN_AC_LUMA : HUFFMAN_AC_CHROMA);
    
    writer.writeBits(table[size].code, table[size].length);
    if (size > 0) {
        writer.writeBits(value & ((1 << size) - 1), size);
    }
}

int decodeVLC(BitReader& reader, bool isDC, bool isLuma) {
    auto& table = isDC ? (isLuma ? HUFFMAN_DC_LUMA : HUFFMAN_DC_CHROMA)
                       : (isLuma ? HUFFMAN_AC_LUMA : HUFFMAN_AC_CHROMA);
    
    uint8_t size = 0;
    for (const auto& entry : table) {
        if (reader.peekBits(entry.length) == entry.code) {
            reader.skipBits(entry.length);
            size = entry.value;
            break;
        }
    }
    if (size == 0) return 0;
    int value = reader.readBits(size);
    if (value < (1 << (size - 1))) value -= (1 << size) - 1;
    return value;
}
