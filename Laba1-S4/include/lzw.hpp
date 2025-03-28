//
//  lzw.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef LZW_H
#define LZW_H

#include <string>
#include <map>
#include <vector>

class LZW {
public:
    LZW(size_t block_size);
    
    std::string encode(const std::string& data);
    std::string decode(const std::string& data);

private:
    size_t block_size;
    size_t dict_size;
    std::map<std::string, uint16_t> dictionary;
    std::map<uint16_t, std::string> rev_dictionary;

    void reset_dict();
    std::string pack_code(uint16_t code);
    uint16_t unpack_code(const std::string& packed);
};

#endif // LZW_H
