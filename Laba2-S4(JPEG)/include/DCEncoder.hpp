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

    // Функция для разностного кодирования DC коэффициентов.
    // Принимает вектор исходных DC коэффициентов (например, полученных из обработки изображения)
    // и возвращает вектор разностей, где первый элемент остается без изменений,
    // а каждый последующий равен разнице между текущим и предыдущим.
    std::vector<int> encodeDC(const std::vector<int>& dcCoeffs);

    // Функция для декодирования DC коэффициентов из разностного кодирования.
    // Принимает вектор, содержащий разностные значения DC коэффициентов,
    // и восстанавливает исходный вектор DC коэффициентов.
    std::vector<int> decodeDC(const std::vector<int>& diffCoeffs);

}


#endif /* DCEncoder_hpp */
