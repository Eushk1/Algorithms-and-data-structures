//
//  DCcoder.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef DCcoder_hpp
#define DCcoder_hpp

#include <vector>
#include "VLC.hpp"

class DCCoder {
public:
    static std::vector<uint8_t> encode(const std::vector<int>& dc_coeffs);
    static std::vector<int> decode(const std::vector<uint8_t>& encoded_data);
private:
    static std::vector<int> compute_deltas(const std::vector<int>& dc_coeffs);
};
#endif /* DCcoder_hpp */
