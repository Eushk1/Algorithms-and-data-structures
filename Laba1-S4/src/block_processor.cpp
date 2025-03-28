//
//  block_processor.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "block_processor.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <algorithm>

const size_t BlockProcessor::BLOCK_HEADER_SIZE = sizeof(uint32_t);
bool BlockProcessor::use_header = true;

std::vector<std::string> BlockProcessor::split_blocks(const std::string& data, size_t block_size) {
    std::vector<std::string> blocks;
    if (block_size == 0) return {data};
    
    for (size_t i = 0; i < data.size(); i += block_size) {
        size_t end = std::min(i + block_size, data.size());
        blocks.push_back(data.substr(i, end - i));
    }
    return blocks;
}

std::string BlockProcessor::add_block_header(const std::string& block) {
    if (use_header) {
        uint32_t block_len = htonl(block.size());
        return std::string(reinterpret_cast<char*>(&block_len), BLOCK_HEADER_SIZE) + block;
    }
    return block;
}

std::pair<std::string, size_t> BlockProcessor::read_block(const std::string& data, size_t ptr) {
    if (!use_header) return {data.substr(ptr), data.size()};

    if (ptr + BLOCK_HEADER_SIZE > data.size()) return {"", ptr};
    
    uint32_t block_len;
    memcpy(&block_len, data.data() + ptr, BLOCK_HEADER_SIZE);
    block_len = ntohl(block_len);
    
    ptr += BLOCK_HEADER_SIZE;
    if (ptr + block_len > data.size()) return {"", ptr};
    
    return {data.substr(ptr, block_len), ptr + block_len};
}
