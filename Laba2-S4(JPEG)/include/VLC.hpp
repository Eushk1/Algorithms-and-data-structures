//
//  VLC.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef VLC_hpp
#define VLC_hpp

#include <vector>
#include <cstdint>

class VLC {
public:
    // Кодирование разностей DC
    static std::vector<uint8_t> encodeDC(const std::vector<int>& dc_deltas);
    
    // Декодирование разностей DC
    static std::vector<int> decodeDC(const std::vector<uint8_t>& encoded_data);
    
    // Кодирование AC-коэффициентов (RLE пары)
    static std::vector<uint8_t> encodeAC(const std::vector<std::pair<int, int>>& ac_rle);
    
    // Декодирование AC-коэффициентов
    static std::vector<std::pair<int, int>> decodeAC(const std::vector<uint8_t>& encoded_data);
};

#endif /* VLC_hpp */
