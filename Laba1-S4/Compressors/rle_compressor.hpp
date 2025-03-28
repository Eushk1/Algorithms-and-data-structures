//
//  rle_compressor.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef RLE_COMPRESSOR_H
#define RLE_COMPRESSOR_H

#include "rle_packbits.hpp"
#include <string>

class RLECompressor {
public:
    static void compress(const std::string& input_file, const std::string& output_file);
    static void decompress(const std::string& input_file, const std::string& output_file);
};

#endif // RLE_COMPRESSOR_H
