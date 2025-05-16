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


    std::string encodeDC(const std::vector<int>& dcDiffs);
    std::vector<int> decodeDC(const std::string& bitstream);


    std::string encodeAC(const std::vector<int>& acCoeffs);
    std::vector<int> decodeAC(const std::string& bitstream);

}

#endif // VLCODER_HPP
