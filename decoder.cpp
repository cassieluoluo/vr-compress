/**
 * decoder.cpp
 *
 * The decoder
 *
 * Author: Luoyili Zhou (luoyiliz@usc.edu)
 *
 */
#include <algorithm>
#include <cstdio>
#include <vector>
#include <opencv2/opencv.hpp>
#include <boost/program_options.hpp>
#include "include/defs.h"
#include "include/cmp_file.h"
#include "include/utils.h"

namespace po = boost::program_options;

int n1 = 10;
int n2 = 80;
bool debug_mode = false;

void quantize(std::vector<short>& data, int step) {
    std::transform(data.begin(), data.end(), data.begin(),
        [step](short val) { return (val / step) * step; });
}

void q(std::vector<short>& data, short v) {
    std::transform(data.begin(), data.end(), data.begin(),
        [v](short val) { return v; });
}
cv::Mat block2Mat(BlockFrame block) {
    std::vector<short> data(block.coeff, block.coeff + 64);
    if (block.type % 2 == 1) {
        if (debug_mode) return cv::Mat(8, 8, CV_8UC1, cv::Scalar(0));
        quantize(data, n2);
    } else {
        if (debug_mode) return cv::Mat(8, 8, CV_8UC1, cv::Scalar(255));
        quantize(data, n1);
    }
    cv::Mat mb(data);
    mb.convertTo(mb, CV_32FC1);
    mb = mb.reshape(0, 8);
    cv::idct(mb, mb);
    return mb;
}

int main(int argc, char **argv) {
    int width;
    int height;
    const std::string WINDOW_NAME = "Decoder";
    std::string filename;
    float frame_rate;
    
    try { 
        po::options_description desc("VR Decoder");
        desc.add_options()
            ("help", "display help message")
            ("width", po::value<int>(&width)->default_value(960), "the width of the video frame")
            ("height",po::value<int>(&height)->default_value(544), "the height of the video frame")
            ("frame-rate", po::value<float>(&frame_rate)->default_value(25.0), "frame rate at which the video plays")
            ("input-file", po::value<std::string>(), "input compressed file")
            ("debug-mode,d", po::bool_switch(&debug_mode), "toggle debug mode")
        ;
        po::positional_options_description p;
        p.add("input-file",  -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);
        if (vm.count("help")) {
            std::cout << desc;
            return 0;
        }
        if (vm.count("input-file")) {
            filename = vm["input-file"].as<std::string>();
        } else {
            std::cerr << "Not input-file provided!" << std::endl;
            return 1;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    CmpFile cmpfile(filename);
    cv::namedWindow(WINDOW_NAME);
    int count = 0;
    while (char c = cv::waitKey((int)(1000.0/frame_rate))) {
        if (c == 'q') break;
        auto data = cmpfile.getNextFrame();
        cv::Mat frame(cv::Size(width, height), CV_8UC3);
        cv::Mat color_block(8, 8, CV_8UC3);
        for (int i = 0; i < data.size(); i += 3) {
            auto ch_red = block2Mat(data[i]);
            auto ch_green = block2Mat(data[i + 1]);
            auto ch_blue = block2Mat(data[i + 2]);
            std::vector<cv::Mat> channels;
            channels.push_back(ch_blue);
            channels.push_back(ch_green);
            channels.push_back(ch_red);
            cv::merge(channels, color_block);
            int row = data[i].row;
            int col = data[i].col;
            color_block.copyTo(frame(cv::Rect(col, row, 8, 8)));
        }
        imshow(WINDOW_NAME, frame);
        std::cout << "frame " << count << std::endl;
        count++;
    }
    return 0;
}