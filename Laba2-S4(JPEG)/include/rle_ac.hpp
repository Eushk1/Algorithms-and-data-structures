//
//  rle_ac.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef rle_ac_hpp
#define rle_ac_hpp

#include <vector>
#include <string>

namespace RLE {

// Кодирует AC коэффициенты с помощью алгоритма RLE.
// Входной параметр: вектор AC коэффициентов.
// Возвращает битовую строку с RLE‑кодированными данными.
std::string encodeAC(const std::vector<int>& acCoeffs);

// Декодирует битовую строку, полученную с помощью RLE‑кодирования, и восстанавливает AC коэффициенты.
// Входной параметр: строка с RLE‑кодированными данными.
// Возвращает вектор восстановленных AC коэффициентов.
std::vector<int> decodeAC(const std::string& bitstream);

} // namespace RLE


#endif /* rle_ac_hpp */
