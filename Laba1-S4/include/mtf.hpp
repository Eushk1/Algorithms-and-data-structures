//
//  mtf.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef MTF_H
#define MTF_H

#include <string>
#include <list>
#include <cstdint>

class MTF {
public:
    MTF(size_t block_size);
    
    std::string encode(const std::string& data);
    std::string decode(const std::string& data);

private:
    size_t block_size;
};

#endif // MTF_H
