//
//  mtf.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "mtf.hpp"
#include "block_processor.hpp"
#include <algorithm>
#include <list>

MTF::MTF(size_t block_size) : block_size(block_size) {}

std::string MTF::encode(const std::string& data) {
    BlockProcessor bp;
    std::string encoded;

    std::list<uint8_t> symbols;
    for (int i = 0; i < 256; ++i) {
        symbols.push_back(static_cast<uint8_t>(i));
    }

    for (const auto& block : bp.split_blocks(data, block_size)) {
        std::string block_enc;
        auto local_symbols = symbols;

        for (uint8_t b : block) {
            auto it = std::find(local_symbols.begin(), local_symbols.end(), b);
            size_t idx = std::distance(local_symbols.begin(), it);

            block_enc.push_back(static_cast<uint8_t>(idx));

            local_symbols.erase(it);
            local_symbols.push_front(b);
        }

        encoded += bp.add_block_header(block_enc);
    }

    return encoded;
}

std::string MTF::decode(const std::string& data) {
    BlockProcessor bp;
    std::string decoded;
    size_t ptr = 0;

    std::list<uint8_t> symbols;
    for (int i = 0; i < 256; ++i) {
        symbols.push_back(static_cast<uint8_t>(i));
    }

    while (ptr < data.size()) {
        auto [block_enc, new_ptr] = bp.read_block(data, ptr);
        ptr = new_ptr;
        if (block_enc.empty()) break;

        auto local_symbols = symbols;

        for (uint8_t idx : block_enc) {
            auto it = std::next(local_symbols.begin(), idx);
            uint8_t b = *it;

            decoded.push_back(b);

            local_symbols.erase(it);
            local_symbols.push_front(b);
        }
    }

    return decoded;
}
