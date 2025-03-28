//
//  rle_packbits.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef RLE_PACKBITS_H
#define RLE_PACKBITS_H

#include <string>
#include <vector>
#include <utility>

class RlePackBits {
public:
    std::string encode(const std::string& block);
    std::string decode(const std::string& block);
};

#endif // RLE_PACKBITS_H
