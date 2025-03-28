//
//  lzw_compressor.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef LZW_COMPRESSOR_H
#define LZW_COMPRESSOR_H

#include "lzw.hpp"
#include <string>

class LZWCompressor {
public:
    static void compress(const std::string& input_file, const std::string& output_file);
    static void decompress(const std::string& input_file, const std::string& output_file);
};

#endif // LZW_COMPRESSOR_H
