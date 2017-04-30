/**
 * cmp_file.cpp
 *
 * Author: Luoyili Zhou (luoyiliz@usc.edu)
 *
 */

#include <iostream>
#include "include/cmp_file.h"

CmpFile::CmpFile(std::string filename, int w, int h) {
    file_stream = std::ifstream(filename,
        std::ios_base::in | std::ios_base::binary);
    width = w;
    height = h;
}

std::vector<BlockFrame> CmpFile::getNextFrame() {
    const int BLOCK_SIZE = 8;
    std::vector<BlockFrame> frame;
    for (int row = 0; row < height; row += BLOCK_SIZE) {
        for (int col = 0; col < width; col += BLOCK_SIZE) {
            for (int ch = 0; ch < 3; ch++) {
                BlockFrame block;
                file_stream.read(reinterpret_cast<char *>(&block),
                    sizeof(BlockFrame));
                frame.push_back(block);
                if (block.row > 544) {
                    std::cerr << "Error**************" << std::endl;
                    exit(0);
                }
            }
        }
    }
    return frame;
}
