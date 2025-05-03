//
//  EntropyCoding.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "EntropyCoding.hpp"
#include <stdexcept>

namespace JPEGEntropy {

// Функция кодирования DC-коэффициента по таблице
std::pair<uint8_t, uint16_t> HuffmanEncoder::encodeDC(uint8_t category,
                                                        ComponentType compType)
{
    // Выбираем таблицу в зависимости от типа компоненты
    if (compType == ComponentType::Luminance) {
        auto it = JPEGHuffmanTables::DC_LUM.find(category);
        if (it != JPEGHuffmanTables::DC_LUM.end())
            return it->second;
    } else {
        auto it = JPEGHuffmanTables::DC_CHR.find(category);
        if (it != JPEGHuffmanTables::DC_CHR.end())
            return it->second;
    }
    throw std::runtime_error("encodeDC: недопустимая категория DC");
}

// Функция кодирования AC-коэффициента по таблице
std::pair<uint8_t, uint16_t> HuffmanEncoder::encodeAC(uint8_t run, uint8_t size,
                                                        ComponentType compType)
{
    std::pair<uint8_t, uint8_t> key = { run, size };
    if (compType == ComponentType::Luminance) {
        auto it = JPEGHuffmanTables::AC_LUM.find(key);
        if (it != JPEGHuffmanTables::AC_LUM.end())
            return it->second;
    } else {
        auto it = JPEGHuffmanTables::AC_CHR.find(key);
        if (it != JPEGHuffmanTables::AC_CHR.end())
            return it->second;
    }
    throw std::runtime_error("encodeAC: недопустимая комбинация (run, size)");
}

// Функция декодирования DC-символа.
// bits содержит битовую последовательность, а bitCount – число значащих бит в bits.
// Функция перебирает элементы соответствующей таблицы и ищет совпадение
// первых length бит (из bits) с кодовым словом.
std::pair<uint8_t, uint8_t> HuffmanDecoder::decodeDC(uint16_t bits, uint8_t bitCount,
                                                       ComponentType compType)
{
    const auto& table = (compType == ComponentType::Luminance)
                        ? JPEGHuffmanTables::DC_LUM : JPEGHuffmanTables::DC_CHR;
    for (const auto& entry : table) {
        uint8_t length = entry.second.first;
        uint16_t code = entry.second.second;
        if (length <= bitCount) {
            // Из bits выделяем старшие length бит (предполагается, что bits "выравнено" в bitCount битах)
            uint16_t mask = bits >> (bitCount - length);
            if (mask == code)
                return { entry.first, length };
        }
    }
    // Если символ не найден, возвращаем специальное значение
    return { 255, 0 };
}

// Декодирование AC-символа по аналогичной схеме.
// Возвращается пара: { {run, size}, затраченные биты }.
std::pair<std::pair<uint8_t, uint8_t>, uint8_t>
HuffmanDecoder::decodeAC(uint16_t bits, uint8_t bitCount, ComponentType compType)
{
    const auto& table = (compType == ComponentType::Luminance)
                        ? JPEGHuffmanTables::AC_LUM : JPEGHuffmanTables::AC_CHR;
    for (const auto& entry : table) {
        uint8_t length = entry.second.first;
        uint16_t code = entry.second.second;
        if (length <= bitCount) {
            uint16_t mask = bits >> (bitCount - length);
            if (mask == code)
                return { { entry.first.first, entry.first.second }, length };
        }
    }
    return { {{255, 255}}, 0 };
}

} // namespace JPEGEntropy
``
