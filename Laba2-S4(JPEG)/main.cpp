#include "JPEGCompressorLib.hpp"
#include <iostream>

int main() {
    std::cout << "Starting JPEG compression and decompression test..." << std::endl;

    // Создаем экземпляр компрессора
    JPEGCompressorLib compressor;

    // Тестовые файлы
    std::string inputFile = "Base_BMP24.bmp";    // Исходное изображение
    std::string compressedFile = "Base_BMP24_comp.bin"; // Сжатый файл
    std::string decompressedFile = "Base_BMP24_output_decompressed.bmp"; // Декомпрессированное изображение

    // Качество сжатия (от 1 до 100, где 100 - наилучшее качество)
    int quality = 80;

    // Тест компрессии
    std::cout << "Compressing " << inputFile << " to " << compressedFile << " with quality " << quality << "..." << std::endl;
    bool compressionSuccess = compressor.compressJPEG(inputFile, compressedFile, quality);
    if (compressionSuccess) {
        std::cout << "Compression successful!" << std::endl;
    } else {
        std::cerr << "Compression failed!" << std::endl;
        return 1;
    }

    // Тест декомпрессии
    std::cout << "Decompressing " << compressedFile << " to " << decompressedFile << "..." << std::endl;
    bool decompressionSuccess = compressor.decompressJPEG(compressedFile, decompressedFile);
    if (decompressionSuccess) {
        std::cout << "Decompression successful!" << std::endl;
    } else {
        std::cerr << "Decompression failed!" << std::endl;
        return 1;
    }

    std::cout << "Test completed. Check the output files to verify the results." << std::endl;
    return 0;
}

