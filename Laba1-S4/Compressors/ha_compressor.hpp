//
//  ha_compressor.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef HA_COMPRESSOR_H
#define HA_COMPRESSOR_H

#include "huffman.h"
#include <string>

class HACompressor {
public:
    static void compress(const std::string& input_file, const std::string& output_file);
    static void decompress(const std::string& input_file, const std::string& output_file);
};

#endif // HA_COMPRESSOR_H
