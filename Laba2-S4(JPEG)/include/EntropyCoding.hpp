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

// Для удобства введём тип компоненты (люминесценция или хроматичность)
enum class ComponentType {
    Luminance,
    Chrominance
};

// Класс для кодирования (энтропийного) по таблицам Хаффмана
class HuffmanEncoder {
public:
    // Кодирование DC-сегмента.
    // category – категория (номер диапазона по количеству бит представления разности DC),
    // compType – тип компоненты (по умолчанию люминесценция).
    // Возвращает пару: (количество бит кода, само кодовое слово)
    static std::pair<uint8_t, uint16_t> encodeDC(uint8_t category,
                                                 ComponentType compType = ComponentType::Luminance);
    
    // Кодирование AC-сегмента.
    // run – количество нулей перед ненулевым коэффициентом,
    // size – количество бит для представления ненулевого коэффициента,
    // compType – тип компоненты.
    // Возвращает пару: (количество бит кода, кодовое слово)
    static std::pair<uint8_t, uint16_t> encodeAC(uint8_t run, uint8_t size,
                                                 ComponentType compType = ComponentType::Luminance);
};

// Класс для декодирования по таблицам Хаффмана
class HuffmanDecoder {
public:
    // Декодирует DC-сегмент. На вход подаётся битовая последовательность,
    // представленная путем: bits – число, содержащее «bitCount» старших бит,
    // bitCount – количество доступных бит.
    // Возвращается пара: {кодовая категория, количество бит, затраченных на кодовое слово}.
    // Если совпадения не найдено, возвращается {255, 0}.
    static std::pair<uint8_t, uint8_t> decodeDC(uint16_t bits, uint8_t bitCount,
                                                  ComponentType compType = ComponentType::Luminance);
    
    // Декодирует AC-сегмент. На вход подаются bits и bitCount аналогично.
    // Возвращает пару: { {run, size}, bitCountConsumed }.
    // Если символ не найден, возвращается { {255, 255}, 0 }.
    static std::pair<std::pair<uint8_t, uint8_t>, uint8_t> decodeAC(uint16_t bits, uint8_t bitCount,
                                                                     ComponentType compType = ComponentType::Luminance);
};

} // namespace JPEGEntropy



#endif /* EntropyCoding_hpp */
