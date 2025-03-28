//
//  lzss.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef LZSS_H
#define LZSS_H

#include <string>
#include <vector>

class LZSS {
public:
    LZSS(size_t block_size, size_t window_size = 2048);
    
    std::string encode(const std::string& data);
    std::string decode(const std::string& data);

private:
    size_t block_size;
    size_t window_size;
};

#endif // LZSS_H
