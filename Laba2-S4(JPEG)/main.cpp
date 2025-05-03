#include "ImageBlockProcessor.hpp"
#include "ImageFileIO.hpp"
#include "ColorSpaceConverter.hpp"
#include "DCT.hpp"
#include "Quantizer.hpp" // Подключаем наш новый модуль
#include "QuantizationTables.hpp" // Если хотим выбирать таблицу в зависимости от канала
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

// Функция для применения DCT/IDCT с этапом квантования/де-квантования по заданному качеству (1-100)
// Здесь для простоты для всех каналов используется таблица для яркости, для цветности можно применять DEFAULT_CHROMA_QUANT_TABLE.
template<typename T>
void processDCTAndIDCT(ImageBlocks<T>& blocks, const std::string& channelName, int quality) {
    auto start = std::chrono::high_resolution_clock::now();

    // Выбираем таблицу квантования:
    // Для канала Y можно использовать DEFAULT_LUMA_QUANT_TABLE,
    // а для Cb и Cr — DEFAULT_CHROMA_QUANT_TABLE, если требуется.
    const auto& quantTable = DEFAULT_LUMA_QUANT_TABLE;

    ImageBlockProcessor::processBlocks(blocks, [quantTable, quality](auto& block) {
        // Преобразуем блок в матрицу double
        std::vector<std::vector<double>> input(block.data.size(), std::vector<double>(block.data[0].size(), 0.0));
        for (size_t i = 0; i < block.data.size(); i++) {
            for (size_t j = 0; j < block.data[i].size(); j++) {
                input[i][j] = static_cast<double>(block.data[i][j]);
            }
        }
        // Прямое DCT
        auto dctResult = dct2d(input);

        // Квантование и немедленное де-квантование с использованием отдельной функции
        auto quantized = applyQuantization(dctResult, quantTable, quality);

        // Обратное DCT (IDCT)
        auto idctResult = idct2d(quantized);

        // Записываем полученные значения обратно в блок с округлением и ограничением диапазона 0–255
        for (size_t i = 0; i < block.data.size(); i++) {
            for (size_t j = 0; j < block.data[i].size(); j++) {
                int val = static_cast<int>(std::round(idctResult[i][j]));
                if (val < 0)   val = 0;
                if (val > 255) val = 255;
                block.data[i][j] = static_cast<T>(val);
            }
        }
    });

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << channelName << " DCT/IDCT with quantization (quality " << quality << "%) processed in "
              << duration.count() << " ms" << std::endl;
}

void testAllChannelsProcessing() {
    try {
        // 1. Загрузка и конвертация
        std::vector<RGBPixel> rgbPixels;
        uint32_t width, height;
        readBMP24("/Users/Egor 1/Desktop/АиСД лаба2/Base_BMP24.bmp", rgbPixels, width, height);
        auto ycbcrPixels = convertRGBToYCbCr(rgbPixels);

        // 2. Подготовка каналов
        std::vector<uint8_t> Y(width * height), Cb(width * height), Cr(width * height);
        for (size_t i = 0; i < ycbcrPixels.size(); i++) {
            Y[i]  = ycbcrPixels[i].Y;
            Cb[i] = ycbcrPixels[i].Cb;
            Cr[i] = ycbcrPixels[i].Cr;
        }

        // 3. Разбиение на блоки
        auto yBlocks  = ImageBlockProcessor::splitToBlocks(Y,  width, height);
        auto cbBlocks = ImageBlockProcessor::splitToBlocks(Cb, width, height);
        auto crBlocks = ImageBlockProcessor::splitToBlocks(Cr, width, height);

        // 4. Применение DCT/IDCT с квантованием для всех блоков каждого канала
        int quality = 100; // Пример уровня качества (в процентах)
        processDCTAndIDCT(yBlocks,  "Y channel", quality);
        processDCTAndIDCT(cbBlocks, "Cb channel", quality);  // Для цветности можно использовать DEFAULT_CHROMA_QUANT_TABLE
        processDCTAndIDCT(crBlocks, "Cr channel", quality);

        // 5. Сборка блоков обратно в векторы и объединение каналов
        Y  = ImageBlockProcessor::combineBlocks(yBlocks,  width, height);
        Cb = ImageBlockProcessor::combineBlocks(cbBlocks, width, height);
        Cr = ImageBlockProcessor::combineBlocks(crBlocks, width, height);

        for (size_t i = 0; i < ycbcrPixels.size(); i++) {
            ycbcrPixels[i] = { Y[i], Cb[i], Cr[i] };
        }

        // 6. Конвертация в RGB и сохранение итогового изображения
        writeBMP24("/Users/Egor 1/Desktop/АиСД лаба2/Base_BMP24_output.bmp", convertYCbCrToRGB(ycbcrPixels), width, height);

        std::cout << "All channels processed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    testAllChannelsProcessing();
    return 0;
}
