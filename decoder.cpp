/**
 * decoder.cpp
 *
 * The decoder
 *
 * Author: Luoyili Zhou (luoyiliz@usc.edu)
 *
 */
#include <algorithm>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cstdio>

#include "include/defs.h"
#include "include/cmp_file.h"
#include "include/utils.h"

int n1 = 20;
int n2 = 40;

void quantize(std::vector<short>& data, int step) {
    std::transform(data.begin(), data.end(), data.begin(),
        [step](short val) { return (val / step) * step; });
}

cv::Mat block2Mat(BlockFrame block) {
    std::vector<short> data(64, 0);
    for (int i = 0; i < 64; i++) {
        data[i] = block.coeff[i];
    }
    // if (block.type == BlockFrame::BACKGROUND) {
    //     quantize(data, n2);
    // } else {
    //     quantize(data, n1);
    // }
    cv::Mat mb(data);
    mb.convertTo(mb, CV_32FC1);
    mb = mb.reshape(0, 8);
    cv::idct(mb, mb);
    //Utils::print_mat(mb);
    return mb;
}

int main(int argc, char **argv) {
    int width = 960;
    int height = 544;
    CmpFile cmpfile("data/oneperson_960_540.rgb.cmp");
    cv::namedWindow("Decoder");
    while (char c = cv::waitKey(20)) {
        if (c == 'q') break;
        auto data = cmpfile.getNextFrame();
        cv::Mat frame(cv::Size(width, height), CV_8UC3);
        for (int i = 0; i < data.size(); i += 3) {
            auto ch_red = block2Mat(data[i]);
            auto ch_green = block2Mat(data[i + 1]);
            auto ch_blue = block2Mat(data[i + 2]);
            std::vector<cv::Mat> channels;
            channels.push_back(ch_red);
            channels.push_back(ch_green);
            channels.push_back(ch_blue);
            cv::Mat color_block;
            cv::merge(channels, color_block);
            int row = data[i].row;
            int col = data[i].col;
            color_block.copyTo(frame(cv::Rect(col, row, 8, 8)));
        }
        imshow("Decoder", frame);
    }
    return 0;
}