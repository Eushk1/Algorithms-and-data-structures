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


std::string encodeAC(const std::vector<int>& acCoeffs);


std::vector<int> decodeAC(const std::string& bitstream);

} // namespace RLE


#endif /* rle_ac_hpp */
