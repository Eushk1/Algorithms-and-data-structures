#include "JPEGCompressorLib.hpp"
#include <fstream>
#include <iostream>


void writeMetadata(std::ofstream& ofs, const JPEGMetadata& metadata) {
    ofs.write(reinterpret_cast<const char*>(&metadata.width), sizeof(metadata.width));
    ofs.write(reinterpret_cast<const char*>(&metadata.height), sizeof(metadata.height));
    ofs.write(reinterpret_cast<const char*>(&metadata.quality), sizeof(metadata.quality));

    for (const auto& row : metadata.quantizationTableLuma) {
        for (int val : row) {
            ofs.write(reinterpret_cast<const char*>(&val), sizeof(val));
        }
    }
    for (const auto& row : metadata.quantizationTableChroma) {
        for (int val : row) {
            ofs.write(reinterpret_cast<const char*>(&val), sizeof(val));
        }
    }


    auto dcLumaTable = metadata.huffmanTables.getDCLumaTable();
    auto acLumaTable = metadata.huffmanTables.getACLumaTable();
    auto dcChromaTable = metadata.huffmanTables.getDCChromaTable();
    auto acChromaTable = metadata.huffmanTables.getACChromaTable();

    for (const auto& entry : dcLumaTable) {
        ofs.write(reinterpret_cast<const char*>(&entry.first), sizeof(entry.first));
        ofs.write(reinterpret_cast<const char*>(&entry.second), sizeof(entry.second));
    }
    for (const auto& entry : acLumaTable) {
        ofs.write(reinterpret_cast<const char*>(&entry.first), sizeof(entry.first));
        ofs.write(reinterpret_cast<const char*>(&entry.second), sizeof(entry.second));
    }
    for (const auto& entry : dcChromaTable) {
        ofs.write(reinterpret_cast<const char*>(&entry.first), sizeof(entry.first));
        ofs.write(reinterpret_cast<const char*>(&entry.second), sizeof(entry.second));
    }
    for (const auto& entry : acChromaTable) {
        ofs.write(reinterpret_cast<const char*>(&entry.first), sizeof(entry.first));
        ofs.write(reinterpret_cast<const char*>(&entry.second), sizeof(entry.second));
    }
}


JPEGMetadata readMetadata(std::ifstream& ifs) {
    JPEGMetadata metadata;
    ifs.read(reinterpret_cast<char*>(&metadata.width), sizeof(metadata.width));
    ifs.read(reinterpret_cast<char*>(&metadata.height), sizeof(metadata.height));
    ifs.read(reinterpret_cast<char*>(&metadata.quality), sizeof(metadata.quality));


    metadata.quantizationTableLuma.resize(8, std::vector<int>(8));
    metadata.quantizationTableChroma.resize(8, std::vector<int>(8));
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            ifs.read(reinterpret_cast<char*>(&metadata.quantizationTableLuma[i][j]), sizeof(int));
        }
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            ifs.read(reinterpret_cast<char*>(&metadata.quantizationTableChroma[i][j]), sizeof(int));
        }
    }


    return metadata;
}

bool JPEGCompressorLib::compressJPEG(const std::string& inputFile, const std::string& outputFile, int quality) {

    ImageData image = ImageFileIO::readBMP(inputFile);
    if (image.data.empty()) {
        std::cerr << "Error reading input file: " << inputFile << std::endl;
        return false;
    }


    YCbCrImage ycbcrImage = ColorSpaceConverter::RGBtoYCbCr(image);


    ChromaSubsampling::downsample(ycbcrImage);


    auto yBlocks = ImageBlockProcessor::splitIntoBlocks(ycbcrImage.Y, 8);
    auto cbBlocks = ImageBlockProcessor::splitIntoBlocks(ycbcrImage.Cb, 8);
    auto crBlocks = ImageBlockProcessor::splitIntoBlocks(ycbcrImage.Cr, 8);


    auto yDCT = DCT::applyDCTBlocks(yBlocks);
    auto cbDCT = DCT::applyDCTBlocks(cbBlocks);
    auto crDCT = DCT::applyDCTBlocks(crBlocks);


    QuantizationTables qt;
    auto quantizationTableLuma = qt.getLumaTable(quality);
    auto quantizationTableChroma = qt.getChromaTable(quality);
    auto yQuantized = Quantizer::quantizeBlocks(yDCT, quantizationTableLuma);
    auto cbQuantized = Quantizer::quantizeBlocks(cbDCT, quantizationTableChroma);
    auto crQuantized = Quantizer::quantizeBlocks(crDCT, quantizationTableChroma);


    auto yDCDiff = DCEncoder::differenceEncode(yQuantized);
    auto cbDCDiff = DCEncoder::differenceEncode(cbQuantized);
    auto crDCDiff = DCEncoder::differenceEncode(crQuantized);


    auto yRLE = RLE_AC::encode(yQuantized);
    auto cbRLE = RLE_AC::encode(cbQuantized);
    auto crRLE = RLE_AC::encode(crQuantized);


    HuffmanTables huffmanTables;
    EntropyCoding ec(huffmanTables);
    auto yEncoded = ec.encode(yDCDiff, yRLE, true);
    auto cbEncoded = ec.encode(cbDCDiff, cbRLE, false);
    auto crEncoded = ec.encode(crDCDiff, crRLE, false);

    std::ofstream ofs(outputFile, std::ios::binary);
    if (!ofs) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return false;
    }

    JPEGMetadata metadata;
    metadata.width = image.width;
    metadata.height = image.height;
    metadata.quality = quality;
    metadata.quantizationTableLuma = quantizationTableLuma;
    metadata.quantizationTableChroma = quantizationTableChroma;
    metadata.huffmanTables = huffmanTables;

    writeMetadata(ofs, metadata);

    ofs.write(reinterpret_cast<const char*>(yEncoded.data()), yEncoded.size());
    ofs.write(reinterpret_cast<const char*>(cbEncoded.data()), cbEncoded.size());
    ofs.write(reinterpret_cast<const char*>(crEncoded.data()), crEncoded.size());

    ofs.close();
    return true;
}

bool JPEGCompressorLib::decompressJPEG(const std::string& inputFile, const std::string& outputFile) {

    std::ifstream ifs(inputFile, std::ios::binary);
    if (!ifs) {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return false;
    }

    JPEGMetadata metadata = readMetadata(ifs);
    std::vector<char> yEncoded((metadata.width * metadata.height) / 64 * 128); //
    std::vector<char> cbEncoded((metadata.width * metadata.height) / 256 * 128);
    std::vector<char> crEncoded((metadata.width * metadata.height) / 256 * 128);

    ifs.read(yEncoded.data(), yEncoded.size());
    ifs.read(cbEncoded.data(), cbEncoded.size());
    ifs.read(crEncoded.data(), crEncoded.size());

    ifs.close();

  
    EntropyCoding ec(metadata.huffmanTables);
    auto yDecoded = ec.decode(yEncoded, true);
    auto cbDecoded = ec.decode(cbEncoded, false); /
    auto crDecoded = ec.decode(crEncoded, false);

 
    auto yQuantized = RLE_AC::decode(yDecoded.second);
    auto cbQuantized = RLE_AC::decode(cbDecoded.second);
    auto crQuantized = RLE_AC::decode(crDecoded.second);


    auto yDC = DCEncoder::differenceDecode(yDecoded.first);
    auto cbDC = DCEncoder::differenceDecode(cbDecoded.first);
    auto crDC = DCEncoder::differenceDecode(crDecoded.first);


    auto yDCT = Quantizer::dequantizeBlocks(yQuantized, metadata.quantizationTableLuma);
    auto cbDCT = Quantizer::dequantizeBlocks(cbQuantized, metadata.quantizationTableChroma);
    auto crDCT = Quantizer::dequantizeBlocks(crQuantized, metadata.quantizationTableChroma);


    auto yBlocks = DCT::applyInverseDCTBlocks(yDCT);
    auto cbBlocks = DCT::applyInverseDCTBlocks(cbDCT);
    auto crBlocks = DCT::applyInverseDCTBlocks(crDCT);


    YCbCrImage ycbcrImage;
    ycbcrImage.Y = ImageBlockProcessor::mergeBlocks(yBlocks, metadata.width, metadata.height);
    ycbcrImage.Cb = ImageBlockProcessor::mergeBlocks(cbBlocks, metadata.width / 2, metadata.height / 2);
    ycbcrImage.Cr = ImageBlockProcessor::mergeBlocks(crBlocks, metadata.width / 2, metadata.height / 2);


    ChromaSubsampling::upsample(ycbcrImage);


    ImageData image = ColorSpaceConverter::YCbCrtoRGB(ycbcrImage);


    bool success = ImageFileIO::writeBMP(outputFile, image);
    if (!success) {
        std::cerr << "Error writing output file: " << outputFile << std::endl;
        return false;
    }

    return true;
}
