#include "ImageFileIO.hpp"
#include "ColorSpaceConverter.hpp"
#include "Downsampler.hpp"
#include "ImageBlockProcessor.hpp"
#include "DCTProcessor.hpp"
#include "Quantization.hpp"
#include <iostream>
#include <chrono>

int main() {
    try {
        std::string basePath = "/Users/Egor 1/Desktop/АиСД лаба2/";
        
        // 1. Чтение исходного BMP изображения
        std::cout << "1. Reading BMP file..." << std::endl;
        std::vector<RGBPixel> rgbPixels;
        uint32_t width, height;
        readBMP24(basePath + "Base_BMP24.bmp", rgbPixels, width, height);
        std::cout << "   Image size: " << width << "x" << height << std::endl;
        
        // 2. Конвертация в YCbCr 4:4:4
        std::cout << "2. Converting to YCbCr 4:4:4..." << std::endl;
        auto ycbcrPixels = convertRGBToYCbCr(rgbPixels);
        writeRawYCbCr(basePath + "01_ycbcr444.raw", ycbcrPixels, width, height);
        
        // 3. Подготовка YCbCrImage
        std::cout << "3. Preparing YCbCr image structure..." << std::endl;
        YCbCrImage ycbcrImage;
        ycbcrImage.width = width;
        ycbcrImage.height = height;
        ycbcrImage.Y.resize(width * height);
        ycbcrImage.Cb.resize(width * height);
        ycbcrImage.Cr.resize(width * height);
        
        for (size_t i = 0; i < ycbcrPixels.size(); i++) {
            ycbcrImage.Y[i] = ycbcrPixels[i].Y;
            ycbcrImage.Cb[i] = ycbcrPixels[i].Cb;
            ycbcrImage.Cr[i] = ycbcrPixels[i].Cr;
        }
        
        // 4. Даунсемплинг до 4:2:0
        std::cout << "4. Downsampling to YCbCr 4:2:0..." << std::endl;
        YCbCrImage ycbcr420 = downsampleYCbCr420(ycbcrImage);
        writeRawYCbCr420(basePath + "02_ycbcr420.raw", ycbcr420);
        
        // 5. Подготовка данных для DCT
        std::cout << "5. Preparing data for DCT..." << std::endl;
        std::vector<float> yChannelFloat(width * height);
        std::vector<float> cbChannelFloat((width/2) * (height/2));
        std::vector<float> crChannelFloat((width/2) * (height/2));
        
        // Конвертация Y канала
        for (size_t i = 0; i < ycbcr420.Y.size(); i++) {
            yChannelFloat[i] = static_cast<float>(ycbcr420.Y[i]) - 128.0f; // Центрирование
        }
        
        // Конвертация Cb и Cr каналов
        for (size_t i = 0; i < ycbcr420.Cb.size(); i++) {
            cbChannelFloat[i] = static_cast<float>(ycbcr420.Cb[i]) - 128.0f;
            crChannelFloat[i] = static_cast<float>(ycbcr420.Cr[i]) - 128.0f;
        }
        
        // 6. Разбиение на блоки 8x8
        std::cout << "6. Splitting into 8x8 blocks..." << std::endl;
        auto yBlocks = splitImageIntoBlocks(yChannelFloat, width, height, BLOCK_SIZE, 0.0f);
        auto cbBlocks = splitImageIntoBlocks(cbChannelFloat, width/2, height/2, BLOCK_SIZE, 0.0f);
        auto crBlocks = splitImageIntoBlocks(crChannelFloat, width/2, height/2, BLOCK_SIZE, 0.0f);
        
        // 7. Применение DCT
        std::cout << "7. Applying DCT..." << std::endl;
        applyDCTToBlocks(yBlocks);
        applyDCTToBlocks(cbBlocks);
        applyDCTToBlocks(crBlocks);
        
        // 8. Подготовка матриц квантования
        std::cout << "8. Preparing quantization tables..." << std::endl;
        QualityFactor quality = QualityFactor::MEDIUM; // MEDIUM = 75
        auto lumaQuantTable = scaleQuantizationTable(DEFAULT_LUMA_QUANT_TABLE, quality);
        auto chromaQuantTable = scaleQuantizationTable(DEFAULT_CHROMA_QUANT_TABLE, quality);
        
        // 9. Квантование коэффициентов
        std::cout << "9. Applying quantization..." << std::endl;
        applyQuantization(yBlocks, lumaQuantTable, chromaQuantTable, true);
        applyQuantization(cbBlocks, lumaQuantTable, chromaQuantTable, false);
        applyQuantization(crBlocks, lumaQuantTable, chromaQuantTable, false);
        
        // 10. Обратное квантование
        std::cout << "10. Applying dequantization..." << std::endl;
        applyDequantization(yBlocks, lumaQuantTable, chromaQuantTable, true);
        applyDequantization(cbBlocks, lumaQuantTable, chromaQuantTable, false);
        applyDequantization(crBlocks, lumaQuantTable, chromaQuantTable, false);
        
        // 11. Обратное DCT
        std::cout << "11. Applying inverse DCT..." << std::endl;
        applyIDCTToBlocks(yBlocks);
        applyIDCTToBlocks(cbBlocks);
        applyIDCTToBlocks(crBlocks);
        
        // 12. Сборка каналов обратно
        std::cout << "12. Reconstructing channels..." << std::endl;
        auto processedYChannel = combineBlocksIntoImage(yBlocks, width, height);
        auto processedCbChannel = combineBlocksIntoImage(cbBlocks, width/2, height/2);
        auto processedCrChannel = combineBlocksIntoImage(crBlocks, width/2, height/2);
        
        // 13. Подготовка данных для обратного преобразования
        for (auto& val : processedYChannel) val += 128.0f;
        for (auto& val : processedCbChannel) val += 128.0f;
        for (auto& val : processedCrChannel) val += 128.0f;
        
        // 14. Апсемплинг цветности
        std::cout << "14. Upsampling chroma channels..." << std::endl;
        YCbCrImage processedImage;
        processedImage.width = width;
        processedImage.height = height;
        processedImage.Y.resize(width * height);
        processedImage.Cb.resize(width * height);
        processedImage.Cr.resize(width * height);
        
        for (size_t i = 0; i < processedYChannel.size(); i++) {
            processedImage.Y[i] = static_cast<uint8_t>(std::clamp(processedYChannel[i], 0.0f, 255.0f));
        }
        
        // Простейший апсемплинг (дублирование пикселей)
        for (int y = 0; y < height/2; y++) {
            for (int x = 0; x < width/2; x++) {
                int idx = y * (width/2) + x;
                uint8_t cb = static_cast<uint8_t>(std::clamp(processedCbChannel[idx], 0.0f, 255.0f));
                uint8_t cr = static_cast<uint8_t>(std::clamp(processedCrChannel[idx], 0.0f, 255.0f));
                
                for (int dy = 0; dy < 2; dy++) {
                    for (int dx = 0; dx < 2; dx++) {
                        int target_idx = (2*y + dy) * width + (2*x + dx);
                        processedImage.Cb[target_idx] = cb;
                        processedImage.Cr[target_idx] = cr;
                    }
                }
            }
        }
        
        // 15. Конвертация обратно в RGB
        std::cout << "15. Converting back to RGB..." << std::endl;
        std::vector<YCbCrPixel> restoredPixels(width * height);
        for (size_t i = 0; i < restoredPixels.size(); i++) {
            restoredPixels[i].Y = processedImage.Y[i];
            restoredPixels[i].Cb = processedImage.Cb[i];
            restoredPixels[i].Cr = processedImage.Cr[i];
        }
        
        auto restoredRGB = convertYCbCrToRGB(restoredPixels);
        writeBMP24(basePath + "03_restored.bmp", restoredRGB, width, height);
        
        std::cout << "Processing completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

