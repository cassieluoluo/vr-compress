#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>


class Utils {
public:
    static void divideImg8by8(const cv::Mat &img, std::vector<cv::Mat> &res);
    static void calculateBlockDCT(const std::vector<cv::Mat> &blocks, std::vector<cv::Mat> &res);
    static void calculateBlockSum(const std::vector<cv::Mat> &blocks, std::vector<int> &res);
};