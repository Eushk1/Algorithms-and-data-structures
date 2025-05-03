// ImageFileIO.hpp
#ifndef IMAGE_FILE_IO_HPP
#define IMAGE_FILE_IO_HPP

#include <string>
#include <cstdint>
#include <vector>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature = 0x4D42;
    uint32_t fileSize;
    uint32_t reserved = 0;
    uint32_t dataOffset = 54;
    uint32_t headerSize = 40;
    int32_t width;
    int32_t height;
    uint16_t planes = 1;
    uint16_t bitsPerPixel;
    uint32_t compression = 0;
    uint32_t imageSize;
    uint32_t xPixelsPerMeter = 0;
    uint32_t yPixelsPerMeter = 0;
    uint32_t colorsUsed = 0;
    uint32_t colorsImportant = 0;
};

struct RawHeader {
    uint32_t width;
    uint32_t height;
    uint16_t bitsPerPixel;
};

struct RGBPixel {
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

struct YCbCrPixel {
    uint8_t Y;
    uint8_t Cb;
    uint8_t Cr;
};

struct YCbCrImage {
    std::vector<uint8_t> Y;
    std::vector<uint8_t> Cb;
    std::vector<uint8_t> Cr;
    uint32_t width;
    uint32_t height;
};
#pragma pack(pop)

// Функции для работы с файлами
void readBMP24(const std::string& path, std::vector<RGBPixel>& pixels, uint32_t& width, uint32_t& height);
void writeBMP24(const std::string& path, const std::vector<RGBPixel>& pixels, uint32_t width, uint32_t height);
void readRawRGB(const std::string& path, std::vector<RGBPixel>& pixels, uint32_t& width, uint32_t& height);
void writeRawRGB(const std::string& path, const std::vector<RGBPixel>& pixels, uint32_t width, uint32_t height);
void writeRawYCbCr(const std::string& path, const std::vector<YCbCrPixel>& pixels, uint32_t width, uint32_t height);
YCbCrImage readRawYCbCr420(const std::string& path);
void writeRawYCbCr420(const std::string& path, const YCbCrImage& image);

#endif // IMAGE_FILE_IO_HPP
