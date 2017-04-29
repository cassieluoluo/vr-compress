#include <exception>
#include <iostream>

#include "include/cmp_file.h"

CmpFile::CmpFile(std::string filename) {
    is_read = true;
    file_stream = std::fstream(filename, std::ios_base::in | std::ios_base::binary);
    file_stream.read((char *)&cmp_header, sizeof(CmpHeader));
    if (cmp_header.magic_number != CmpFile::MAGIC_NUMBER) {
        throw std::exception(); // Need to figure out how to add message to exception
    }
}

CmpFile::CmpFile(std::string filename, int width, int height, int frame_rate) {
    is_read = false;
    file_stream = std::fstream(filename, std::ios_base::out | std::ios_base::binary);
    cmp_header.width = width;
    cmp_header.height = height;
    cmp_header.frame_rate = (unsigned short)frame_rate;
    file_stream.seekg(sizeof(CmpHeader));
}

std::vector<CompressedBlock> CmpFile::getNextFrame() {
    std::vector<CompressedBlock> frame;
    for (int i = 0; i < cmp_header.width; i++) {
        for (int j = 0; j < cmp_header.height; j++) {
            CompressedBlock block;
            file_stream.read(reinterpret_cast<char *>(&block), sizeof(CompressedBlock));
            frame.push_back(block);
        }
    }
    return frame;
}

void CmpFile::writeFrame(std::vector<CompressedBlock> frame) {
    for (auto it = frame.begin(); it != frame.end(); ++it) {
        file_stream.write(reinterpret_cast<char *>(&(*it)), sizeof(CompressedBlock));
    }
}

void CmpFile::finishWriting() {
    file_stream.seekg(std::ios_base::beg);
    file_stream.write(reinterpret_cast<char *>(&cmp_header), sizeof(CmpHeader));
    file_stream.close();
}