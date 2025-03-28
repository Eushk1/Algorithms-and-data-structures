//
//  lzw.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "lzw.hpp"
#include "block_processor.hpp"
#include <cstdint>

LZW::LZW(size_t block_size) : block_size(block_size) {
    reset_dict();
}

std::string LZW::encode(const std::string& data) {
    BlockProcessor bp;
    std::string encoded;

    for (const auto& block : bp.split_blocks(data, block_size)) {
        reset_dict();
        std::string w;
        std::string block_enc;

        for (char c : block) {
            std::string wc = w + c;
            if (dictionary.find(wc) != dictionary.end()) {
                w = wc;
            } else {
                block_enc.append(pack_code(dictionary[w]));
                if (dict_size < 65536) {
                    dictionary[wc] = dict_size++;
                }
                w = std::string(1, c);
            }
        }

        if (!w.empty()) {
            block_enc.append(pack_code(dictionary[w]));
        }

        encoded += bp.add_block_header(block_enc);
    }

    return encoded;
}

std::string LZW::decode(const std::string& data) {
    BlockProcessor bp;
    std::string decoded;
    size_t ptr = 0;

    while (ptr < data.size()) {
        auto [block_enc, new_ptr] = bp.read_block(data, ptr);
        ptr = new_ptr;
        if (block_enc.empty()) break;

        reset_dict();
        size_t idx = 0;

        if (block_enc.size() < 2) continue;


        uint16_t code = unpack_code(block_enc.substr(idx, 2));
        idx += 2;
        if (rev_dictionary.find(code) == rev_dictionary.end()) continue;
        std::string prev = rev_dictionary[code];
        decoded += prev;


        while (idx < block_enc.size()) {
            if (idx + 2 > block_enc.size()) break;
            code = unpack_code(block_enc.substr(idx, 2));
            idx += 2;

            std::string entry;
            if (rev_dictionary.find(code) != rev_dictionary.end()) {
                entry = rev_dictionary[code];
            } else if (code == dict_size) {
                entry = prev + prev[0];
            } else {
                break;
            }

            decoded += entry;
            if (dict_size < 65536) { 
                rev_dictionary[dict_size++] = prev + entry[0];
            }
            prev = entry;
        }
    }

    return decoded;
}

void LZW::reset_dict() {
    dict_size = 256;
    dictionary.clear();
    rev_dictionary.clear();
    for (int i = 0; i < 256; ++i) {
        std::string s(1, static_cast<char>(i));
        dictionary[s] = i;
        rev_dictionary[i] = s;
    }
}

std::string LZW::pack_code(uint16_t code) {
    std::string packed(2, '\0');
    packed[0] = static_cast<char>((code >> 8) & 0xFF);
    packed[1] = static_cast<char>(code & 0xFF);
    return packed;
}

uint16_t LZW::unpack_code(const std::string& packed) {
    return (static_cast<uint8_t>(packed[0]) << 8) | static_cast<uint8_t>(packed[1]);
}
