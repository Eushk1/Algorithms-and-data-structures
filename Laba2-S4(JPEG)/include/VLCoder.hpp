//
//  VLCoder.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef VLCODER_HPP
#define VLCODER_HPP

#include <string>
#include <vector>

namespace VLCoder {

    /*
     * Функции для переменного кодирования DC коэффициентов.
     * encodeDC: принимает вектор, содержащий разности DC коэффициентов,
     *           возвращает строку с переменно-кодированным битовым представлением.
     * decodeDC: принимает битовый поток в виде строки,
     *           возвращает восстановленный вектор разностей DC коэффициентов.
     */
    std::string encodeDC(const std::vector<int>& dcDiffs);
    std::vector<int> decodeDC(const std::string& bitstream);

    /*
     * Функции для переменного кодирования AC коэффициентов.
     * encodeAC: принимает вектор AC коэффициентов (например, длины 63 для блока)
     *           и возвращает строку с переменно-кодированным битовым представлением.
     * decodeAC: принимает битовый поток в виде строки,
     *           возвращает восстановленный вектор AC коэффициентов.
     */
    std::string encodeAC(const std::vector<int>& acCoeffs);
    std::vector<int> decodeAC(const std::string& bitstream);

}

#endif // VLCODER_HPP
