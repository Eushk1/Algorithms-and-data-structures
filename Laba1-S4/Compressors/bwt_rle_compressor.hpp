//
//  bwt_rle_compressor.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef BWT_RLE_COMPRESSOR_H
#define BWT_RLE_COMPRESSOR_H

#include "bwt.h"
#include "rle_packbits.h"
#include <string>

class BWTRLECompressor {
public:
    static void compress(const std::string& input_file, const std::string& output_file);
    static void decompress(const std::string& input_file, const std::string& output_file);
};

#endif // BWT_RLE_COMPRESSOR_H
