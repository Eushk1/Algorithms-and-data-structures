//
//  bwt.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef BWT_H
#define BWT_H

#include <vector>
#include <string>
#include <cstdint>

typedef int32_t saidx_t;
typedef uint8_t uchar;

class BWT {
public:
    static std::vector<saidx_t> divsufsort_optimized(const std::vector<uchar>& T);
    static void compute_optimized_bwt(const std::string& input_file, const std::string& output_file);
    static void inverse_optimized_bwt(const std::string& input_file, const std::string& output_file);
};

#endif // BWT_H
