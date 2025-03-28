//
//  utils.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

std::vector<uint8_t> dynamic_len_encode_2byte(long number, bool firstBit);
std::vector<uint8_t> dynamic_len_encode_1byte(long number, bool firstBit);
std::pair<bool, long> dynamic_len_decode_2byte(const std::vector<uint8_t>& bytes);
std::pair<long, bool> dynamic_len_decode_1byte(const std::vector<uint8_t>& bytes);
std::string readFileAsString(const std::string& filename);

#endif // UTILS_H
