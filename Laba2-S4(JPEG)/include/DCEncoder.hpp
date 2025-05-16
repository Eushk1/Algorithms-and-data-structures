//
//  DCEncoder.hpp
//  AaSD_Jpeg_laba2
//
//  Created by Егор on 03.05.2025.
//

#ifndef DCEncoder_hpp
#define DCEncoder_hpp


#include <vector>

namespace DCCoder {


    std::vector<int> encodeDC(const std::vector<int>& dcCoeffs);


    std::vector<int> decodeDC(const std::vector<int>& diffCoeffs);

}


#endif /* DCEncoder_hpp */
