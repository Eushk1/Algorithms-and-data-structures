//
//  bwt_rle_compressor.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "bwt_rle_compressor.hpp"
#include "utils.hpp"
#include <fstream>
#include <chrono>
#include <iostream>

void BWTRLECompressor::compress(const std::string& input_file, const std::string& output_file) {
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        std::string data = readFileAsString(input_file);
        
        std::string bwt_transformed = BWT::bwt(data);
        
        RlePackBits rle;
        std::string compressed = rle.encode(bwt_transformed);
        
        std::ofstream out(output_file, std::ios::binary);
        out.write(compressed.data(), compressed.size());
        out.close();
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        
        std::cout << "Compression successful (" << elapsed.count() << "s)\n";
        std::cout << "Original size: " << data.size() << " bytes\n";
        std::cout << "Compressed size: " << compressed.size() << " bytes\n";
        std::cout << "Compression ratio: "
                  << (double)data.size() / compressed.size() << ":1\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void BWTRLECompressor::decompress(const std::string& input_file, const std::string& output_file) {
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        std::string compressed = readFileAsString(input_file);
        
        RlePackBits rle;
        std::string bwt_transformed = rle.decode(compressed);
        
        std::string decompressed = BWT::inverse_bwt(bwt_transformed);
        
        std::ofstream out(output_file, std::ios::binary);
        out.write(decompressed.data(), decompressed.size());
        out.close();
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        
        std::cout << "Decompression successful (" << elapsed.count() << "s)\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
