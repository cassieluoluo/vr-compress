/**
 * vr_player.cpp
 *
 * VR Player
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
#include "include/decoder.h"
#include "include/utils.h"

bool paused;

namespace po = boost::program_options;

void mouseCallback(int event, int x, int y, int flag, void *param) {
    if (event == cv::EVENT_MOUSEMOVE) {
        Decoder::mouse_x = x;
        Decoder::mouse_y = y;
    } else if (event == cv::EVENT_LBUTTONUP) {
        std::cout << "mouse clicked" << std::endl;
        paused = !paused;
    }
}

int main(int argc, char **argv) {
    const std::string WINDOW_NAME = "VR Player";
    int width, height;
    float frame_rate;
    bool debug_mode = false;
    int foreground_step, background_step;
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
                po::value<int>(&background_step)->default_value(40),
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

    Decoder decoder(filename, width, height, foreground_step, background_step);
    decoder.toggleDebugMode(debug_mode);
    cv::namedWindow(WINDOW_NAME);
    cv::setMouseCallback(WINDOW_NAME, mouseCallback);
    int count = 0;
    while (char c = cv::waitKey((int)(1000.0/frame_rate))) {
        if (c == 'q') break;
        if (!paused) {
            int row, col;
            std::vector<BlockFrame> data;
            auto frame = decoder.getNextFrame();
            cv::imshow(WINDOW_NAME, frame);
            std::cout << "frame " << count << std::endl;
            count++;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
