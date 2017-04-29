#include <exception>
#include <iostream>

#include "include/cmp_file.h"

CmpFile::CmpFile(std::string filename, int w, int h) {
    file_stream = std::ifstream(filename, std::ios_base::in | std::ios_base::binary);
    width = w;
    height = h;
}

// CmpFile::CmpFile(std::string filename, int width, int height, int frame_rate) {
//     is_read = false;
//     file_stream = std::fstream(filename, std::ios_base::out | std::ios_base::binary);
//     cmp_header.width = width;
//     cmp_header.height = height;
//     cmp_header.frame_rate = (unsigned short)frame_rate;
//     file_stream.seekg(sizeof(CmpHeader));
// }

std::vector<BlockFrame> CmpFile::getNextFrame() {
    const int BLOCK_SIZE = 8;
    std::vector<BlockFrame> frame;
    for (int row = 0; row < height; row += BLOCK_SIZE) {
        for (int col = 0; col < width; col += BLOCK_SIZE) {
            BlockFrame block;
            file_stream.read(reinterpret_cast<char *>(&block), sizeof(BlockFrame));
            frame.push_back(block);
        }
    }
    return frame;
}

// void CmpFile::writeFrame(std::vector<CompressedBlock> frame) {
//     for (auto it = frame.begin(); it != frame.end(); ++it) {
//         file_stream.write(reinterpret_cast<char *>(&(*it)), sizeof(CompressedBlock));
//     }
// }

// void CmpFile::finishWriting() {
//     file_stream.seekg(std::ios_base::beg);
//     file_stream.write(reinterpret_cast<char *>(&cmp_header), sizeof(CmpHeader));
//     file_stream.close();
// }