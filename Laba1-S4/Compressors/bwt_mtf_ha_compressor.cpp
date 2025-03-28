//
//  bwt_mtf_ha_compressor.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "bwt_mtf_ha_compressor.hpp"
#include "utils.hpp"
#include <fstream>
#include <chrono>
#include <iostream>

void BWTMTFHACompressor::compress(const std::string& input_file, const std::string& output_file) {
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        std::string data = readFileAsString(input_file);
        
        // Apply BWT
        std::string bwt_transformed = BWT::bwt(data);
        
        // Apply MTF
        MTF mtf(0); // 0 means process whole file as one block
        std::string mtf_transformed = mtf.encode(bwt_transformed);
        
        // Apply Huffman
        Huffman huffman(0);
        std::string compressed = huffman.encode(mtf_transformed);
        
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

void BWTMTFHACompressor::decompress(const std::string& input_file, const std::string& output_file) {
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        std::string compressed = readFileAsString(input_file);
        
        // Apply Huffman decode
        Huffman huffman(0);
        std::string mtf_transformed = huffman.decode(compressed);
        
        // Apply MTF decode
        MTF mtf(0);
        std::string bwt_transformed = mtf.decode(mtf_transformed);
        
        // Apply inverse BWT
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
