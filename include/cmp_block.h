#pragma once

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class CompressedBlock {
public:
    CompressedBlock(std::vector<short>& c, bool type);
    void encode(cv::Mat block);
    cv::Mat decode(int step, bool gazing);
    std::vector<short> bytes();
    unsigned char type();

    static const int BLOCK_SIZE = 8;
private:
    bool block_type; // true: foreground, false: background
    std::vector<short> coeffs;

    cv::Mat quantize(int step);
};
