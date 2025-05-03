//
//  Bitstream.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "Bitstream.hpp"

void BitWriter::writeBit(bool bit) {
    currentByte |= (bit << (7 - bitsWritten));
    bitsWritten++;
    if (bitsWritten == 8) {
        buffer.push_back(currentByte);
        currentByte = 0;
        bitsWritten = 0;
    }
}

void BitWriter::writeBits(uint32_t code, int length) {
    for (int i = length - 1; i >= 0; i--) {
        writeBit((code >> i) & 1);
    }
}

void BitWriter::flush() {
    if (bitsWritten > 0) {
        buffer.push_back(currentByte);
        currentByte = 0;
        bitsWritten = 0;
    }
}

BitReader::BitReader(const std::vector<uint8_t>& data) : buffer(data) {}

bool BitReader::readBit() {
    if (bytePos >= buffer.size()) throw std::out_of_range("BitReader overflow");
    bool bit = (buffer[bytePos] >> bitPos) & 1;
    bitPos--;
    if (bitPos < 0) {
        bytePos++;
        bitPos = 7;
    }
    return bit;
}

uint32_t BitReader::readBits(int length) {
    uint32_t result = 0;
    for (int i = 0; i < length; i++) {
        result = (result << 1) | readBit();
    }
    return result;
}
