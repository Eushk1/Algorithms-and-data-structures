//
//  Bitstream.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef Bitstream_hpp
#define Bitstream_hpp

#include <vector>
#include <cstdint>

class BitWriter {
    std::vector<uint8_t> buffer;
    uint8_t currentByte = 0;
    int bitsWritten = 0;

public:
    void writeBit(bool bit);
    void writeBits(uint32_t code, int length);
    void flush(); // Принудительно записать неполный байт
    const std::vector<uint8_t>& getBuffer() const;
};

class BitReader {
    const std::vector<uint8_t>& buffer;
    size_t bytePos = 0;
    int bitPos = 7; // Читаем старшие биты сначала

public:
    BitReader(const std::vector<uint8_t>& data);
    bool readBit();
    uint32_t readBits(int length);
};

#endif /* Bitstream_hpp */
