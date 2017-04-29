#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "defs.h"

class CmpFile {
public:
    CmpFile(std::string filename);  // read
    CmpFile(std::string filename, int width, int height, int frame_rate);   // write
    std::vector<CompressedBlock> getNextFrame();
    bool hasNextFrame();
    void writeFrame(std::vector<CompressedBlock> frame);
    static const unsigned short MAGIC_NUMBER = 0xABCD;

private:
    std::fstream file_stream;
    CmpHeader cmp_header;
    int frame_counter;
    bool is_read;
};
