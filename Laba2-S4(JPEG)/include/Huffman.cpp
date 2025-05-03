//
//  Huffman.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "Huffman.hpp"

void HuffmanEncoder::encodeDC(int diff, bool isLuma, BitWriter& writer) {
    int size = (diff == 0) ? 0 : (int)std::log2(abs(diff)) + 1;
    auto& table = isLuma ? HUFFMAN_DC_LUMA : HUFFMAN_DC_CHROMA;
    writer.writeBits(table[size].code, table[size].length);
    if (size > 0) {
        writer.writeBits(diff & ((1 << size) - 1), size);
    }
}

int HuffmanDecoder::decodeDC(BitReader& reader, bool isLuma) {
    auto& table = isLuma ? HUFFMAN_DC_LUMA : HUFFMAN_DC_CHROMA;
    uint8_t size = 0;
    // Поиск кода в таблице (реализуется аналогично encodeVLC)
    // ...
    if (size == 0) return 0;
    int value = reader.readBits(size);
    if (value < (1 << (size - 1))) value -= (1 << size) - 1;
    return value;
}
