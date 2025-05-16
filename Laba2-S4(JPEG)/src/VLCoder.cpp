//
//  VLCoder.cpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#include "VLCoder.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>


static std::string intToBinary(unsigned int value, int bits) {
    std::string result;
    for (int i = bits - 1; i >= 0; i--) {
        result.push_back(((value >> i) & 1) ? '1' : '0');
    }
    return result;
}


static unsigned int binaryToInt(const std::string &bits) {
    unsigned int value = 0;
    for (char bit : bits) {
        value = (value << 1) + (bit - '0');
    }
    return value;
}


static int getCategory(int value) {
    if (value == 0)
        return 0;
    int absVal = std::abs(value);
    return static_cast<int>(std::floor(std::log2(absVal))) + 1;
}

namespace VLCoder {


    std::string encodeDC(const std::vector<int>& dcDiffs) {
        std::string bitstream;
        for (int diff : dcDiffs) {
            int category = getCategory(diff);

            bitstream += intToBinary(category, 4);
            if (category > 0) {
                unsigned int additional;
                if (diff < 0) {
                    additional = static_cast<unsigned int>(diff + ((1 << category) - 1));
                } else {
                    additional = static_cast<unsigned int>(diff);
                }

                bitstream += intToBinary(additional, category);
            }
        }
        return bitstream;
    }


    std::vector<int> decodeDC(const std::string& bitstream) {
        std::vector<int> dcDiffs;
        size_t index = 0;
        while (index + 4 <= bitstream.size()) {

            std::string catStr = bitstream.substr(index, 4);
            index += 4;
            int category = static_cast<int>(binaryToInt(catStr));
            int diff = 0;
            if (category > 0) {
                if(index + category > bitstream.size()) break;
                std::string additionalStr = bitstream.substr(index, category);
                index += category;
                unsigned int additional = binaryToInt(additionalStr);
                int threshold = 1 << (category - 1);
                if (additional < static_cast<unsigned int>(threshold)) {
                    diff = static_cast<int>(additional) - ((1 << category) - 1);
                } else {
                    diff = static_cast<int>(additional);
                }
            }
            dcDiffs.push_back(diff);
        }
        return dcDiffs;
    }


    std::string encodeAC(const std::vector<int>& acCoeffs) {
        std::string bitstream;
        int zeroCount = 0;
        for (size_t i = 0; i < acCoeffs.size(); i++) {
            int coeff = acCoeffs[i];
            if (coeff == 0) {
                zeroCount++;
                if (zeroCount == 16) {
                    // ZRL код, сбрасываем счетчик
                    bitstream += "11110000";
                    zeroCount = 0;
                }
            } else {
                int category = getCategory(coeff);
                unsigned int firstByte = ((zeroCount & 0xF) << 4) | (category & 0xF);
                bitstream += intToBinary(firstByte, 8);
                if (category > 0) {
                    unsigned int additional;
                    if (coeff < 0) {
                        additional = static_cast<unsigned int>(coeff + ((1 << category) - 1));
                    } else {
                        additional = static_cast<unsigned int>(coeff);
                    }
                    bitstream += intToBinary(additional, category);
                }
                zeroCount = 0;
            }
        }

        bitstream += "00000000";
        return bitstream;
    }


    std::vector<int> decodeAC(const std::string& bitstream) {
        std::vector<int> acCoeffs;
        size_t index = 0;
        while (index + 8 <= bitstream.size()) {
            std::string byteStr = bitstream.substr(index, 8);
            index += 8;

            if (byteStr == "00000000") {
                break;
            }

            if (byteStr == "11110000") {
                for (int i = 0; i < 16; i++) {
                    acCoeffs.push_back(0);
                }
                continue;
            }

            int runLength = static_cast<int>(binaryToInt(byteStr.substr(0, 4)));
            int category = static_cast<int>(binaryToInt(byteStr.substr(4, 4)));

            for (int i = 0; i < runLength; i++) {
                acCoeffs.push_back(0);
            }
            int coeff = 0;
            if (category > 0) {
                if(index + category > bitstream.size()) break; 
                std::string additionalStr = bitstream.substr(index, category);
                index += category;
                unsigned int additional = binaryToInt(additionalStr);
                int threshold = 1 << (category - 1);
                if (additional < static_cast<unsigned int>(threshold)) {
                    coeff = static_cast<int>(additional) - ((1 << category) - 1);
                } else {
                    coeff = static_cast<int>(additional);
                }
            }
            acCoeffs.push_back(coeff);
        }
        return acCoeffs;
    }

} // namespace VLCoder
