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
#include <chrono>
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
    int roi_size;
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
                po::value<int>(&foreground_step)->default_value(8),
                "quantization step size for foreground blocks")
            ("background-step,n",
                po::value<int>(&background_step)->default_value(128),
                "quantization step size for background blocks")
            ("frame-rate,r", po::value<float>(&frame_rate)->default_value(25.0),
                "frame rate at which the video plays")
            ("roi-size,s", po::value<int>(&roi_size)->default_value(32),
                "Gaze control ROI size")
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

    Decoder decoder(filename, width, height, foreground_step, background_step, roi_size);
    decoder.toggleDebugMode(debug_mode);
    cv::namedWindow(WINDOW_NAME);
    cv::setMouseCallback(WINDOW_NAME, mouseCallback);
    int count = 0;
    double sum = 0.0;
    while (1) {
        auto start = std::chrono::high_resolution_clock::now();
        if (!paused) {
            auto frame = decoder.getNextFrame();
            cv::imshow(WINDOW_NAME, frame);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto interval = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double interval_f = interval.count() / 1000.0;
        sum += interval_f;
        printf("frame %d\tinterval %f ms\n", count++, interval_f);
        int wait_time = interval_f > 35.0 ? 1 : (int)(40.0 - interval_f);
        int c = cv::waitKey(wait_time);
        if (c == 'q') {
            printf("average processing time = %f\n", sum / count);
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
