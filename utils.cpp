#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace std;

#include <boost/program_options.hpp>

#include "include/utils.h"

void Utils::divideImg8by8(const cv::Mat &img, std::vector<cv::Mat> &res) {
    res.clear();
    cv::Mat tmp = img;
    for (int i = 0; i < 4; ++i) {
        tmp.push_back(cv::Mat::zeros(1, 960, img.type()));
    }
    for (int row = 0; row < tmp.size().height; row += 8) {
        for (int col = 0; col < tmp.size().width; col += 8) {
            res.push_back(tmp(cv::Range(row, row + 7), cv::Range(col, col + 7)));
        }
    }
    return;
}

void Utils::calculateBlocksSum(const std::vector<cv::Mat> &blocks, std::vector<int> &res) {
    res.clear();
    for(auto it = blocks.begin(); it != blocks.end(); it++) {
        int x = (int)cv::sum(*it.base())[0];
        res.push_back(x);
    }
}

void Utils::getColorFromBlock(const cv::Mat &block, cv::Mat &res, Color color) {
    res = cv::Mat(8, 8, CV_32FC1);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            res.at<float>(i, j) = block.at<cv::Vec3b>(i, j)[color];
        }
    }
}

void Utils::calculateBlockDCT(const cv::Mat &blocks, cv::Mat &res) {
    cv::Mat tmp;
    cv::dct(blocks, tmp, 0);
    tmp.convertTo(res, CV_16S);
}

void Utils::calculateBlocksDCT(const std::vector<cv::Mat> &blocks, std::vector<cv::Mat> &res) {
    res.clear();
    for(auto it = blocks.begin(); it != blocks.end(); it++) {
        cv::Mat dctResult;
        cv::dct(*it.base(), dctResult, 0);
        res.push_back(cv::Mat(dctResult));
    }
}

void Utils::print_vector(std::vector<short> v) {
    for (auto s : v) {
        std::cout << s << ",";
    }
    std::cout << std::endl;
}

void Utils::print_mat(cv::Mat m) {
    std::cout << m << std::endl;
}