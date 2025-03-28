//
//  bwt_mtf_ha_compressor.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef BWT_MTF_HA_COMPRESSOR_H
#define BWT_MTF_HA_COMPRESSOR_H

#include "bwt.hpp"
#include "mtf.hpp"
#include "huffman.hpp"
#include <string>

class BWTMTFHACompressor {
public:
    static void compress(const std::string& input_file, const std::string& output_file);
    static void decompress(const std::string& input_file, const std::string& output_file);
};

#endif // BWT_MTF_HA_COMPRESSOR_H
