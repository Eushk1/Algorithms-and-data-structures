//
//  utils.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "utils.hpp"

#include <stdexcept>

std::vector<uint8_t> dynamic_len_encode_2byte(long number, bool firstBit) {
    std::vector<uint8_t> result;

    if (number < 0) {
        throw std::invalid_argument("Number must be non-negative");
    }


    uint8_t firstByte = (firstBit ? 0x80 : 0x00);
    firstByte |= (number & 0x3F);
    number >>= 6;

    if (number > 0) {
        firstByte |= 0x40;
        result.push_back(firstByte);

        uint8_t secondByte = (number & 0xFF);
        result.push_back(secondByte);
    } else {
        result.push_back(firstByte);
    }

    return result;
}

std::vector<uint8_t> dynamic_len_encode_1byte(long number, bool firstBit) {
    std::vector<uint8_t> result;

    if (number < 0) {
        throw std::invalid_argument("Number must be non-negative");
    }

    uint8_t firstByte = (firstBit ? 0x80 : 0x00);
    firstByte |= (number & 0x7F);

    result.push_back(firstByte);

    return result;
}

std::pair<bool, long> dynamic_len_decode_2byte(const std::vector<uint8_t>& bytes) {
    if (bytes.empty()) {
        throw std::invalid_argument("Input bytes array is empty");
    }

    uint8_t firstByte = bytes[0];
    bool firstBit = (firstByte & 0x80) != 0;
    long number = firstByte & 0x3F;
    int shift = 6;

    if (firstByte & 0x40) {
        if (bytes.size() < 2) {
            throw std::invalid_argument("Missing second byte");
        }
        uint8_t secondByte = bytes[1];
        number |= (static_cast<long>(secondByte) << shift);
    }

    return {firstBit, number};
}

std::pair<long, bool> dynamic_len_decode_1byte(const std::vector<uint8_t>& bytes) {
    bool firstBit = (bytes[0] & 0x80) != 0;
    long number = bytes[0] & 0x7F;
    return {number, firstBit};
}

std::string readFileAsString(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open file " + filename);
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string content(size, '\0');
    if (!file.read(&content[0], size)) {
        throw std::runtime_error("Error: could not read file " + filename);
    }

    return content;
}
