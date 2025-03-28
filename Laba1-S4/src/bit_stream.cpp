//
//  bit_stream.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "bit_stream.hpp"
#include <stdexcept>

BitStream::BitStream(const std::string& filename, const std::string& m)
    : fs(filename, m == "r" ? std::ios::in | std::ios::binary : std::ios::out | std::ios::binary),
      buffer(0), bitPos(0), mode(m) {
    if (!fs.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}

BitStream::~BitStream() {
    close();
}

bool BitStream::eof() const {
    return fs.eof();
}

void BitStream::writeBit(bool bit) {
    if (bit) buffer |= (1 << (7 - bitPos));
    bitPos++;
    if (bitPos == 8) {
        fs.write(reinterpret_cast<char*>(&buffer), 1);
        buffer = 0;
        bitPos = 0;
    }
}

bool BitStream::readBit() {
    if (bitPos == 0) {
        if (!fs.read(reinterpret_cast<char*>(&buffer), 1)) return false;
    }
    bool bit = buffer & (1 << (7 - bitPos));
    bitPos = (bitPos + 1) % 8;
    return bit;
}

void BitStream::flush() {
    if (bitPos > 0) {
        fs.write(reinterpret_cast<char*>(&buffer), 1);
        buffer = 0;
        bitPos = 0;
    }
}

void BitStream::close() {
    if (mode == "w") flush();
    fs.close();
}
