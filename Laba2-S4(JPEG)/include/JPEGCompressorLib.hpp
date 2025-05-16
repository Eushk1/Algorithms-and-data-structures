//
//  JPEGCompressorLib.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef JPEG_COMPRESSOR_LIB_HPP
#define JPEG_COMPRESSOR_LIB_HPP

#include <string>
#include <vector>
#include "ImageFileIO.hpp"
#include "ColorSpaceConverter.hpp"
#include "ChromaSubsampling.hpp"
#include "ImageBlockProcessor.hpp"
#include "DCT.hpp"
#include "Quantizer.hpp"
#include "DCEncoder.hpp"
#include "rle_ac.hpp"
#include "VLCoder.hpp"
#include "EntropyCoding.hpp"
#include "QuantizationTables.hpp"
#include "HuffmanTables.hpp"


struct JPEGMetadata {
    int width;
    int height;
    int quality;
    std::vector<std::vector<int>> quantizationTableLuma;
    std::vector<std::vector<int>> quantizationTableChroma;
    JPEGHuffmanTables::HuffmanTable huffmanTables;
};

class JPEGCompressorLib {
public:

    static bool compressJPEG(const std::string& inputFile, const std::string& outputFile, int quality);


    static bool decompressJPEG(const std::string& inputFile, const std::string& outputFile);
};

#endif // JPEG_COMPRESSOR_LIB_HPP

