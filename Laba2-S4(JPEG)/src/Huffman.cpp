//
//  Huffman.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "Huffman.hpp"
#include <cmath>
#include <stdexcept>

// ============= Кодирование =============
void Huffman::encodeDC(BitWriter& writer, int value, bool isLuma) {
    int cat = getCategory(value);
    const auto& table = isLuma ? JPEGHuffmanTables::DC_LUM : JPEGHuffmanTables::DC_CHR;
    
    auto it = table.find(cat);
    if (it == table.end()) throw std::runtime_error("Invalid DC category");
    
    writer.writeBits(it->second.second, it->second.first);
    
    if (cat > 0) {
        int adjustedValue = value < 0 ? value + (1 << cat) - 1 : value;
        writer.writeBits(adjustedValue, cat);
    }
}

void Huffman::encodeAC(BitWriter& writer, const std::vector<std::pair<int, int>>& acCoeffs, bool isLuma) {
    const auto& table = isLuma ? JPEGHuffmanTables::AC_LUM : JPEGHuffmanTables::AC_CHR;
    
    for (const auto& [run, val] : acCoeffs) {
        if (run == 0 && val == 0) { // EOB
            writer.writeBits(0x00, 4); // Пример для таблицы K.5
            break;
        }
        
        int size = val == 0 ? 0 : getCategory(val);
        auto key = std::make_pair(run, size);
        
        auto it = table.find(key);
        if (it == table.end()) throw std::runtime_error("Invalid AC Huffman code");
        
        writer.writeBits(it->second.second, it->second.first);
        
        if (size > 0) {
            int adjustedVal = val < 0 ? val + (1 << size) - 1 : val;
            writer.writeBits(adjustedVal, size);
        }
    }
}

// ============= Декодирование =============
int Huffman::decodeDC(BitReader& reader, bool isLuma) {
    const auto& table = isLuma ? JPEGHuffmanTables::DC_LUM : JPEGHuffmanTables::DC_CHR;
    int cat = decodeValue<int>(reader, table);
    return readValue(reader, cat);
}

std::vector<std::pair<int, int>> Huffman::decodeAC(BitReader& reader, bool isLuma) {
    const auto& table = isLuma ? JPEGHuffmanTables::AC_LUM : JPEGHuffmanTables::AC_CHR;
    std::vector<std::pair<int, int>> acCoeffs;
    
    while (acCoeffs.size() < 63) {
        auto [run, size] = decodeValue<std::pair<uint8_t, uint8_t>>(reader, table);
        
        if (run == 0 && size == 0) break; // EOB
        
        if (run == 15 && size == 0) { // ZRL
            acCoeffs.emplace_back(15, 0);
            continue;
        }
        
        int val = readValue(reader, size);
        acCoeffs.emplace_back(run, val);
    }
    
    return acCoeffs;
}

// ============= Вспомогательные методы =============
int Huffman::getCategory(int value) {
    if (value == 0) return 0;
    return static_cast<int>(std::log2(std::abs(value))) + 1;
}

int Huffman::readValue(BitReader& reader, int cat) {
    if (cat == 0) return 0;
    
    int val = reader.readBits(cat);
    if (val < (1 << (cat-1))) {
        val -= (1 << cat) - 1;
    }
    return val;
}

template<typename T>
T Huffman::decodeValue(BitReader& reader, const std::unordered_map<T, std::pair<uint8_t, uint16_t>>& table) {
    uint32_t code = 0;
    uint8_t len = 0;
    
    while (len < 16) {
        code = (code << 1) | reader.readBit();
        len++;
        
        for (const auto& [val, codeInfo] : table) {
            if (codeInfo.first == len && codeInfo.second == code) {
                return val;
            }
        }
    }
    
    throw std::runtime_error("Invalid Huffman code");
}
