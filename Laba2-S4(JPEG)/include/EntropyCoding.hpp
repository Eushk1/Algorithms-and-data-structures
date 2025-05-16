//
//  EntropyCoding.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef EntropyCoding_hpp
#define EntropyCoding_hpp

#include <cstdint>
#include <utility>
#include "HuffmanTables.hpp"  // Подключаем наши табличные коды

namespace JPEGEntropy {


enum class ComponentType {
    Luminance,
    Chrominance
};


class HuffmanEncoder {
public:

    static std::pair<uint8_t, uint16_t> encodeDC(uint8_t category,
                                                 ComponentType compType = ComponentType::Luminance);
    

    static std::pair<uint8_t, uint16_t> encodeAC(uint8_t run, uint8_t size,
                                                 ComponentType compType = ComponentType::Luminance);
};


class HuffmanDecoder {
public:

    static std::pair<uint8_t, uint8_t> decodeDC(uint16_t bits, uint8_t bitCount,
                                                  ComponentType compType = ComponentType::Luminance);
    

    static std::pair<std::pair<uint8_t, uint8_t>, uint8_t> decodeAC(uint16_t bits, uint8_t bitCount,
                                                                     ComponentType compType = ComponentType::Luminance);
};

} // namespace JPEGEntropy



#endif /* EntropyCoding_hpp */
