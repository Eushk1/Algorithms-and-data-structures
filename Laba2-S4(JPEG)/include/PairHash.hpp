//
//  PairHash.h
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef PAIR_HASH_HPP
#define PAIR_HASH_HPP

#include <utility>

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2; 
    }
};

#endif // PAIR_HASH_HPP

