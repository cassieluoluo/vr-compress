/**
 * encoder.h
 *
 * Author: Luoyili Zhou (luoyiliz@usc.edu)
 *
 */

#pragma once

#include <opencv2/opencv.hpp>

#include "defs.h"

class Codec {
public:
    Codec();
    ~Codec();
    cv::Mat encodeBlock(cv::Mat block);
    cv::Mat decodeBlock(cv::Mat block);
private:

};