//
//  block_processor.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef BLOCK_PROCESSOR_H
#define BLOCK_PROCESSOR_H

#include <string>
#include <vector>
#include <utility>

class BlockProcessor {
public:
    static const size_t BLOCK_HEADER_SIZE;
    static bool use_header;

    static std::vector<std::string> split_blocks(const std::string& data, size_t block_size);
    static std::string add_block_header(const std::string& block);
    static std::pair<std::string, size_t> read_block(const std::string& data, size_t ptr);
};

#endif // BLOCK_PROCESSOR_H
