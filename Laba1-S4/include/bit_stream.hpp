//
//  bit_stream.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef BIT_STREAM_H
#define BIT_STREAM_H

#include <fstream>
#include <string>
#include <cstdint>

class BitStream {
private:
    std::fstream fs;
    uint8_t buffer;
    int bitPos;
    std::string mode;

public:
    BitStream(const std::string& filename, const std::string& m);
    ~BitStream();
    
    bool eof() const;
    void writeBit(bool bit);
    bool readBit();
    void flush();
    void close();
};

#endif // BIT_STREAM_H
