#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

class CompressedBlock {
public:
    CompressedBlock();
    void encode(cv::Mat block);
    cv::Mat decode(int q_level);
    std::vector<unsigned char> bytes();
    unsigned char type();
private:
    bool block_type; // true: foreground, false: background
    std::vector<unsigned char> coeffs;

}