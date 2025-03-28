//
//  lzw_ha_compressor.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "lzw_ha_compressor.hpp"
#include "utils.hpp"
#include <fstream>
#include <chrono>
#include <iostream>

void LZWHACompressor::compress(const std::string& input_file, const std::string& output_file) {
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        std::string data = readFileAsString(input_file);
        
        LZW lzw(0);
        std::string lzw_compressed = lzw.encode(data);
        
        Huffman huffman(0);
        std::string compressed = huffman.encode(lzw_compressed);
        
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

void LZWHACompressor::decompress(const std::string& input_file, const std::string& output_file) {
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        std::string compressed = readFileAsString(input_file);
        
        Huffman huffman(0);
        std::string lzw_compressed = huffman.decode(compressed);
        
        LZW lzw(0);
        std::string decompressed = lzw.decode(lzw_compressed);
        
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
