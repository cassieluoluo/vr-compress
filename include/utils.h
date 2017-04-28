#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>



class Utils {
public:
    enum Color {
        COLOR_RED = 2,
        COLOR_GREEN = 1,
        COLOR_BLUE = 0
    };
    static void divideImg8by8(const cv::Mat &img, std::vector<cv::Mat> &res);
    static void calculateBlocksSum(const std::vector<cv::Mat> &blocks, std::vector<int> &res);
    static void getColorFromBlock(const cv::Mat &block, cv::Mat &res, Color color);
    static void calculateBlockDCT(const cv::Mat &block, cv::Mat &res);
    static void calculateBlocksDCT(const std::vector<cv::Mat> &blocks, std::vector<cv::Mat> &res);
};