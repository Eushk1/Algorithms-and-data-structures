//
//  EntropyCoding.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "EntropyCoding.hpp"
#include <stdexcept>

namespace JPEGEntropy {

std::pair<uint8_t, uint16_t> HuffmanEncoder::encodeDC(uint8_t category,
                                                        ComponentType compType)
{

    if (compType == ComponentType::Luminance) {
        auto it = JPEGHuffmanTables::DC_LUM.find(category);
        if (it != JPEGHuffmanTables::DC_LUM.end())
            return it->second;
    } else {
        auto it = JPEGHuffmanTables::DC_CHR.find(category);
        if (it != JPEGHuffmanTables::DC_CHR.end())
            return it->second;
    }
    throw std::runtime_error("encodeDC: недопустимая категория DC");
}


std::pair<uint8_t, uint16_t> HuffmanEncoder::encodeAC(uint8_t run, uint8_t size,
                                                        ComponentType compType)
{
    std::pair<uint8_t, uint8_t> key = { run, size };
    if (compType == ComponentType::Luminance) {
        auto it = JPEGHuffmanTables::AC_LUM.find(key);
        if (it != JPEGHuffmanTables::AC_LUM.end())
            return it->second;
    } else {
        auto it = JPEGHuffmanTables::AC_CHR.find(key);
        if (it != JPEGHuffmanTables::AC_CHR.end())
            return it->second;
    }
    throw std::runtime_error("encodeAC: недопустимая комбинация (run, size)");
}


std::pair<uint8_t, uint8_t> HuffmanDecoder::decodeDC(uint16_t bits, uint8_t bitCount,
                                                       ComponentType compType)
{
    const auto& table = (compType == ComponentType::Luminance)
                        ? JPEGHuffmanTables::DC_LUM : JPEGHuffmanTables::DC_CHR;
    for (const auto& entry : table) {
        uint8_t length = entry.second.first;
        uint16_t code = entry.second.second;
        if (length <= bitCount) {

            uint16_t mask = bits >> (bitCount - length);
            if (mask == code)
                return { entry.first, length };
        }
    }

    return { 255, 0 };
}


std::pair<std::pair<uint8_t, uint8_t>, uint8_t>
HuffmanDecoder::decodeAC(uint16_t bits, uint8_t bitCount, ComponentType compType)
{
    const auto& table = (compType == ComponentType::Luminance)
                        ? JPEGHuffmanTables::AC_LUM : JPEGHuffmanTables::AC_CHR;
    for (const auto& entry : table) {
        uint8_t length = entry.second.first;
        uint16_t code = entry.second.second;
        if (length <= bitCount) {
            uint16_t mask = bits >> (bitCount - length);
            if (mask == code)
                return { { entry.first.first, entry.first.second }, length };
        }
    }

    return { { static_cast<uint8_t>(255), static_cast<uint8_t>(255) },
             static_cast<uint8_t>(0) };
}

} // namespace JPEGEntropy
