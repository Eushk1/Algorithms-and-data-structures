//
//  rle_packbits.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "rle_packbits.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>

std::string RlePackBits::encode(const std::string& block) {
    std::string encoded;
    size_t i = 0;

    while (i < block.size()) {
        char byte = block[i];
        size_t count = 1;

        while (i + 1 < block.size() && block[i + 1] == byte) {
            count++;
            i++;
        }

        if (count > 1) {
            long flag=0;
            while (count > 0){
                size_t chunk_length = std::min(static_cast<size_t>(16382), count);
                
                std::vector<uint8_t> len_code = dynamic_len_encode_1byte(chunk_length, false);
                
                for (auto byte : len_code) {
                    encoded.push_back(static_cast<char>(byte));
                }
                
                encoded.push_back(byte);
                
                count = count - chunk_length;
                flag++;
            }
            if(flag>1){
                std::cout << "Encoded (even) times: " << flag << std::endl;
            }
        } else {
            long flag=0;
            size_t start = i;
            
            while (i + 2 < block.size() && block[i + 1] != block[i] && block[i + 2] != block[i+1]) {
                i++;
            }
            
            size_t length = i - start + 1;

            while (length > 0) {
                size_t chunk_length = std::min(static_cast<size_t>(127), length);
                
                std::vector<uint8_t> len_code = dynamic_len_encode_1byte(chunk_length, true);
                
                for (auto byte : len_code) {
                    encoded.push_back(static_cast<char>(byte));
                }
               
                for (size_t j = 0; j < chunk_length; j++) {
                    encoded.push_back(block[start + j]);
                }
                
                start += chunk_length;
                length -= chunk_length;
                flag++;
            }
            if(flag>1){
                std::cout << "Encoded (odd) times: " << flag << std::endl;
            }
        }
        i++;
    }
    return encoded;
}

std::string RlePackBits::decode(const std::string& block) {
    std::string decoded;
    size_t i = 0;

    while (i < block.size()) {
        std::vector<uint8_t> result;
        result.clear();
        
        int8_t counter = static_cast<int8_t>(block[i]);
        result.push_back(counter);
        
        if (((result[0] & 0b01000000) != 0) && ((result[0] & 0b10000000) == 0)) {
            i++;
            int8_t nextByte = static_cast<uint8_t>(block[i]);
            result.push_back(nextByte);
        }
        
        if((result[0] & 0b10000000) != 0){
            auto bytes_decoded = dynamic_len_decode_1byte(result);
            long len = bytes_decoded.first;
            
            for (long j = 0; j < len; j++) {
                decoded.push_back(block[i + 1 + j]);
            }
            i += 1 + len;
        } else {
            auto bytes_decoded = dynamic_len_decode_1byte(result);
            long len = bytes_decoded.second;
            
            for (long j = 0; j < len; j++) {
                decoded.push_back(block[i + 1]);
            }
            i += 2;
        }
    }
    return decoded;
}
