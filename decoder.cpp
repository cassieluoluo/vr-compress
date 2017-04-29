/**
 * decoder.cpp
 *
 * The decoder
 *
 * Author: Luoyili Zhou (luoyiliz@usc.edu)
 *
 */
#include <algorithm>
#include <opencv2/opencv.hpp>

#include "include/defs.h"
#include "include/cmp_file.h"

int main(int argc, char **argv) {
    CmpFile cmpfile("filename.cmp");
    cv::namedWindow("Decoder");
    while (cmpfile.hasNextFrame()) {
        auto frame = cmpfile.getNextFrame();
        cv::Mat rgbframe;
        for (auto& block : frame) {
            rgbframe.push_back(block.decode(10, true));
        }
        rgbframe = rgbframe.reshape(0, 540);
        cv::imshow("Decoder", rgbframe);
    }
    return 0;
}