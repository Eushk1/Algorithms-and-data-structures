// ImageFileIO.cpp
#include "ImageFileIO.hpp"
#include <fstream>
#include <iostream>
#include <vector>

void readBMP24(const std::string& path, std::vector<RGBPixel>& pixels, uint32_t& width, uint32_t& height) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Error opening input file: " + path);
    }

    BMPHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.signature != 0x4D42 || header.bitsPerPixel != 24) {
        throw std::runtime_error("Not a 24-bit BMP file: " + path);
    }

    width = header.width;
    height = header.height;
    pixels.resize(width * height);

    in.seekg(header.dataOffset, std::ios::beg);

    uint32_t rowSize = ((width * 3 + 3) & ~3);
    std::vector<uint8_t> rowData(rowSize);

    for (uint32_t y = 0; y < height; y++) {
        in.read(reinterpret_cast<char*>(rowData.data()), rowSize);
        for (uint32_t x = 0; x < width; x++) {
            pixels[y * width + x] = {
                rowData[x * 3 + 2],  // R
                rowData[x * 3 + 1],  // G
                rowData[x * 3]      // B
            };
        }
    }
}

void writeBMP24(const std::string& path, const std::vector<RGBPixel>& pixels, uint32_t width, uint32_t height) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Error opening output file: " + path);
    }

    BMPHeader header;
    header.width = width;
    header.height = height;
    header.bitsPerPixel = 24;
    uint32_t rowSize = ((width * 3 + 3) & ~3);
    header.imageSize = rowSize * height;
    header.fileSize = header.dataOffset + header.imageSize;

    out.write(reinterpret_cast<char*>(&header), sizeof(header));

    std::vector<uint8_t> rowData(rowSize, 0);
    uint8_t padByte = 0;

    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            const auto& pixel = pixels[y * width + x];
            rowData[x * 3] = pixel.B;
            rowData[x * 3 + 1] = pixel.G;
            rowData[x * 3 + 2] = pixel.R;
        }
        out.write(reinterpret_cast<char*>(rowData.data()), rowSize);
    }
}

void readRawRGB(const std::string& path, std::vector<RGBPixel>& pixels, uint32_t& width, uint32_t& height) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Error opening input file: " + path);
    }

    RawHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.bitsPerPixel != 24) {
        throw std::runtime_error("Not a 24-bit RAW file: " + path);
    }

    width = header.width;
    height = header.height;
    pixels.resize(width * height);
    in.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(RGBPixel));
}

void writeRawRGB(const std::string& path, const std::vector<RGBPixel>& pixels, uint32_t width, uint32_t height) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Error opening output file: " + path);
    }

    RawHeader header;
    header.width = width;
    header.height = height;
    header.bitsPerPixel = 24;

    out.write(reinterpret_cast<char*>(&header), sizeof(header));
    out.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(RGBPixel));
}

void writeRawYCbCr(const std::string& path, const std::vector<YCbCrPixel>& pixels, uint32_t width, uint32_t height) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Error opening output file: " + path);
    }

    RawHeader header;
    header.width = width;
    header.height = height;
    header.bitsPerPixel = 24;

    out.write(reinterpret_cast<char*>(&header), sizeof(header));
    out.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(YCbCrPixel));
}

YCbCrImage readRawYCbCr420(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Error opening input file: " + path);
    }

    RawHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.bitsPerPixel != 24) {
        throw std::runtime_error("Not a 24-bit RAW file: " + path);
    }

    YCbCrImage image;
    image.width = header.width;
    image.height = header.height;
    
    // Читаем YCbCr 4:4:4
    std::vector<YCbCrPixel> pixels(header.width * header.height);
    in.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(YCbCrPixel));
    
    // Разделяем на компоненты
    image.Y.resize(header.width * header.height);
    image.Cb.resize(header.width * header.height);
    image.Cr.resize(header.width * header.height);
    
    for (size_t i = 0; i < pixels.size(); i++) {
        image.Y[i] = pixels[i].Y;
        image.Cb[i] = pixels[i].Cb;
        image.Cr[i] = pixels[i].Cr;
    }
    
    return image;
}

void writeRawYCbCr420(const std::string& path, const YCbCrImage& image) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Error opening output file: " + path);
    }

    RawHeader header;
    header.width = image.width;
    header.height = image.height;
    header.bitsPerPixel = 24;

    out.write(reinterpret_cast<char*>(&header), sizeof(header));
    
    // Для формата 4:2:0 записываем полный Y и даунсемпленные Cb/Cr
    // Сначала Y компонента
    out.write(reinterpret_cast<const char*>(image.Y.data()), image.Y.size());
    
    // Затем Cb и Cr компоненты (уже даунсемпленные)
    uint32_t cbCrWidth = (image.width + 1) / 2;
    uint32_t cbCrHeight = (image.height + 1) / 2;
    
    out.write(reinterpret_cast<const char*>(image.Cb.data()), cbCrWidth * cbCrHeight);
    out.write(reinterpret_cast<const char*>(image.Cr.data()), cbCrWidth * cbCrHeight);
}
