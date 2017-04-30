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

bool debug_mode = false;
int foreground_step = 1;
int background_step = 80;
int mouse_x = -100;
int mouse_y = -100;

void mouse_callback(int event, int x, int y, int flag, void *param) {
    if (event == cv::EVENT_MOUSEMOVE) {
        mouse_x = x;
        mouse_y = y;
    }
}

void quantize(std::vector<short>& data, int step) {
    std::transform(data.begin(), data.end(), data.begin(),
        [step](short val) { return (val / step) * step; });
}

cv::Mat block2Mat(BlockFrame block) {
    std::vector<short> data(block.coeff, block.coeff + 64);
    bool in_roi = block.row - mouse_y > -32
               && block.row - mouse_y < 32
               && block.col - mouse_x > -32
               && block.col - mouse_x < 32;
    if (block.type % 2 == 0 || in_roi) {
        if (debug_mode) return cv::Mat(8, 8, CV_8UC1, cv::Scalar(224));
        quantize(data, foreground_step);
    } else {
        if (debug_mode) return cv::Mat(8, 8, CV_8UC1, cv::Scalar(32));
        quantize(data, background_step);
    }
    cv::Mat mb(data);
    mb.convertTo(mb, CV_32FC1);
    mb = mb.reshape(0, 8);
    cv::idct(mb, mb);
    return mb;
}

int main(int argc, char **argv) {
    const std::string WINDOW_NAME = "Decoder";
    int width, height;
    float frame_rate;
    std::string filename;
    try { 
        po::options_description desc(WINDOW_NAME);
        desc.add_options()
            ("help,h", "display help message")
            ("input-file,i", po::value<std::string>(),
                "input compressed file")
            ("width,w", po::value<int>(&width)->default_value(960),
                "the width of the video frame")
            ("height,h",po::value<int>(&height)->default_value(544),
                "the height of the video frame")
            ("foreground-step,m",
                po::value<int>(&foreground_step)->default_value(1),
                "quantization step size for foreground blocks")
            ("background-step,n",
                po::value<int>(&background_step)->default_value(80),
                "quantization step size for background blocks")
            ("frame-rate,r", po::value<float>(&frame_rate)->default_value(25.0),
                "frame rate at which the video plays")
            ("debug-mode,d", po::bool_switch(&debug_mode),
                "toggle debug mode")
        ;
        po::positional_options_description p;
        p.add("input-file", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc)
            .positional(p).run(), vm);
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
    cv::setMouseCallback(WINDOW_NAME, mouse_callback);
    int count = 0;
    while (char c = cv::waitKey((int)(1000.0/frame_rate))) {
        int row, col;
        std::vector<BlockFrame> data;
        cv::Mat frame;
        try {
            if (c == 'q') break;
            data = cmpfile.getNextFrame();
            frame = cv::Mat(cv::Size(width, height), CV_8UC3);
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
                row = data[i].row;
                col = data[i].col;
                if (row > 544) std::cerr << "Error !!!!!!!!!!!!" << std::endl;
                color_block.copyTo(frame(cv::Rect(col, row, 8, 8)));
            }
        } catch (cv::Exception& e) {
            std::cerr << "cv::Exception" << std::endl;
            std::cerr << e.what() << std::endl;
            printf("col=%d, row=%d, data.size()=%d\n", col, row, data.size());
            return 1;
        }
        cv::imshow(WINDOW_NAME, frame);
        std::cout << "frame " << count << std::endl;
        count++;
    }
    cv::destroyAllWindows();
    return 0;
}
