//
//  lzss.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "lzss.hpp"
#include "block_processor.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <algorithm>

LZSS::LZSS(size_t block_size, size_t window_size)
    : block_size(block_size), window_size(window_size) {}

std::string LZSS::encode(const std::string& data) {
    BlockProcessor bp;
    std::string encoded;

    for (const auto& block : bp.split_blocks(data, block_size)) {
        std::string block_enc;
        uint32_t window_size_net = htonl(window_size);
        block_enc.append(reinterpret_cast<const char*>(&window_size_net), sizeof(window_size_net));

        size_t i = 0;
        while (i < block.size()) {
            std::pair<size_t, size_t> best = {0, 0};
            size_t window_start = (i >= window_size) ? (i - window_size) : 0;
            size_t max_length = std::min(static_cast<size_t>(258), block.size() - i);

            for (size_t l = max_length; l >= 3; --l) {
                std::string substr = block.substr(i, l);
                size_t pos = std::string::npos;
                
                for (size_t p = window_start; p < i; ++p) {
                    if (block.compare(p, l, substr) == 0) {
                        pos = p;
                        break;
                    }
                }
                
                if (pos != std::string::npos) {
                    best = {i - pos, l};
                    break;
                }
            }

            if (best.second >= 3) {
                uint32_t packed = ((best.first - 1) << 12) | (best.second - 3);
                block_enc.push_back(1);
                block_enc.push_back((packed >> 16) & 0xFF);
                block_enc.push_back((packed >> 8) & 0xFF);
                block_enc.push_back(packed & 0xFF);
                i += best.second;
            } else {
                block_enc.push_back(0);
                block_enc.push_back(block[i]);
                ++i;
            }
        }

        encoded += bp.add_block_header(block_enc);
    }

    return encoded;
}

std::string LZSS::decode(const std::string& data) {
    BlockProcessor bp;
    std::string decoded;
    size_t ptr = 0;

    while (ptr < data.size()) {
        auto [block_enc, new_ptr] = bp.read_block(data, ptr);
        ptr = new_ptr;
        if (block_enc.empty()) break;

        uint32_t window_size_net;
        memcpy(&window_size_net, block_enc.data(), sizeof(window_size_net));
        size_t window_size = ntohl(window_size_net);

        size_t i = sizeof(window_size_net);
        std::string buf;

        while (i < block_enc.size()) {
            if (block_enc[i] == 0) { // Literal
                if (i + 1 >= block_enc.size()) break;
                buf.push_back(block_enc[i + 1]);
                i += 2;
            } else if (block_enc[i] == 1) { // Match
                if (i + 4 > block_enc.size()) break;
                
                uint32_t packed = (static_cast<uint8_t>(block_enc[i + 1]) << 16)
                                 | (static_cast<uint8_t>(block_enc[i + 2]) << 8)
                                 | static_cast<uint8_t>(block_enc[i + 3]);
                
                size_t offset = (packed >> 12) + 1; 
                size_t length = (packed & 0xFFF) + 3;
                
                i += 4;

                if (offset > buf.size()) {
                    std::cerr << "Invalid offset: " << offset << " (buffer size: " << buf.size() << ")" << std::endl;
                    continue;
                }

                size_t start = buf.size() - offset;
                for (size_t j = 0; j < length; ++j) {
                    if (start + j >= buf.size()) break;
                    buf.push_back(buf[start + j]);
                }
            } else {
                break;
            }
        }

        decoded += buf;
    }

    return decoded;
}
