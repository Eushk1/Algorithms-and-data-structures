//
//  lzss_ha_compressor.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef LZSS_HA_COMPRESSOR_H
#define LZSS_HA_COMPRESSOR_H

#include "lzss.hpp"
#include "huffman.hpp"
#include <string>

class LZSSHACompressor {
public:
    static void compress(const std::string& input_file, const std::string& output_file);
    static void decompress(const std::string& input_file, const std::string& output_file);
};

#endif // LZSS_HA_COMPRESSOR_H
