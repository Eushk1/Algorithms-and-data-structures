//
//  bwt.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "bwt.hpp"
#include "bit_stream.hpp"
#include <algorithm>
#include <fstream>
#include <vector>

template<typename T>
void ss_insertionsort(const T *Td, const T *PA, T *first, T *last, int d) {
    for (T *i = first + 1; i < last; ++i) {
        T tmp = *i;
        T *j = i - 1;
        while (j >= first && (Td[PA[*j] + d] > Td[PA[tmp] + d])) {
            *(j + 1) = *j;
            --j;
        }
        *(j + 1) = tmp;
    }
}

std::vector<saidx_t> BWT::divsufsort_optimized(const std::vector<uchar>& T) {
    saidx_t n = static_cast<saidx_t>(T.size());
    std::vector<saidx_t> SA(n);
    std::vector<saidx_t> buckets(ALPHABET_SIZE, 0);
    
    for (saidx_t i = 0; i < n; ++i) {
        buckets[T[i]]++;
    }
    
    saidx_t sum = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        saidx_t temp = buckets[i];
        buckets[i] = sum;
        sum += temp;
    }
    
    for (saidx_t i = 0; i < n; ++i) {
        SA[buckets[T[i]]++] = i;
    }
    
    for (int h = 1; h < n; h *= 2) {
        std::vector<saidx_t> newSA(n);
        std::vector<saidx_t> rank(n);
        
        rank[SA[0]] = 0;
        for (saidx_t i = 1; i < n; ++i) {
            rank[SA[i]] = rank[SA[i - 1]];
            if (T[SA[i]] != T[SA[i - 1]] ||
                (SA[i] + h < n ? T[SA[i] + h] : -1) != (SA[i - 1] + h < n ? T[SA[i - 1] + h] : -1)) {
                rank[SA[i]]++;
            }
        }
        
        std::vector<saidx_t> count(n, 0);
        for (saidx_t i = 0; i < n; ++i) {
            count[rank[i]]++;
        }
        
        saidx_t pos = 0;
        for (saidx_t i = 0; i < n; ++i) {
            saidx_t temp = count[i];
            count[i] = pos;
            pos += temp;
        }
        
        for (saidx_t i = 0; i < n; ++i) {
            newSA[count[rank[SA[i]]]++] = SA[i];
        }
        
        SA = std::move(newSA);
    }
    
    return SA;
}

void BWT::compute_optimized_bwt(const std::string& input_file, const std::string& output_file) {
    std::ifstream in(input_file, std::ios::binary);
    std::vector<uchar> T((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    
    T.push_back(0);
    auto SA = divsufsort_optimized(T);
    
    BitStream bit_out(output_file, "w");
    saidx_t primary = 0;
    for (saidx_t i = 0; i < SA.size(); ++i) {
        if (SA[i] == 0) {
            primary = i;
            continue;
        }
        uchar c = T[SA[i] - 1];
        for (int j = 7; j >= 0; --j) {
            bit_out.writeBit((c >> j) & 1);
        }
    }
    
    std::ofstream meta(output_file + ".meta", std::ios::binary);
    meta.write(reinterpret_cast<char*>(&primary), sizeof(saidx_t));
    meta.close();
    bit_out.close();
}

void BWT::inverse_optimized_bwt(const std::string& input_file, const std::string& output_file) {
    saidx_t primary;
    std::ifstream meta(input_file + ".meta", std::ios::binary);
    meta.read(reinterpret_cast<char*>(&primary), sizeof(saidx_t));
    meta.close();
    
    BitStream bit_in(input_file, "r");
    std::vector<uchar> bwt;
    uchar current = 0;
    int bit_count = 0;
    
    while (true) {
        bool bit = bit_in.readBit();
        if (bit_in.eof()) break;
        
        current |= (bit << (7 - bit_count));
        bit_count++;
        
        if (bit_count == 8) {
            bwt.push_back(current);
            current = 0;
            bit_count = 0;
        }
    }
    bit_in.close();
    
    std::vector<saidx_t> LF(bwt.size());
    std::vector<saidx_t> count(ALPHABET_SIZE, 0);
    
    for (uchar c : bwt) {
        count[c]++;
    }
    
    saidx_t sum = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        saidx_t temp = count[i];
        count[i] = sum;
        sum += temp;
    }
    
    std::vector<saidx_t> occ(ALPHABET_SIZE, 0);
    for (saidx_t i = 0; i < bwt.size(); ++i) {
        uchar c = bwt[i];
        LF[i] = count[c] + occ[c]++;
    }
    
    std::vector<uchar> original;
    saidx_t pos = primary;
    for (saidx_t i = 0; i < bwt.size() - 1; ++i) {
        pos = LF[pos];
        original.push_back(bwt[pos]);
    }
    
    std::ofstream out(output_file, std::ios::binary);
    out.write(reinterpret_cast<char*>(original.data()), original.size());
    out.close();
}
