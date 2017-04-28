#include <fstream>
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include "include/raw_video.h"
int mouse_x = 0;
int mouse_y = 0;
void mouse_callback(int  event, int  x, int  y, int  flag, void *param)
{
    
    if (event == cv::EVENT_MOUSEMOVE) {
        mouse_x = x;
        mouse_y = y;
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
}

int main(int argc, char **argv) {
    int width, height;
    const std::string WINDOW_NAME = "Basic Video Player";
    float frame_rate;
    bool pause = false;
    std::string filename;
    try { 
        po::options_description desc("A basic video player that plays raw rgb video");
        desc.add_options()
            ("help", "display help message")
            ("width", po::value<int>(&width)->default_value(960), "the width of the video frame")
            ("height",po::value<int>(&height)->default_value(540), "the height of the video frame")
            ("frame-rate", po::value<float>(&frame_rate)->default_value(25.0), "frame rate at which the video plays")
            ("input-file", po::value<std::string>(), "input video file")
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
    RawVideo rawVideo(filename, width, height);
    cv::namedWindow(WINDOW_NAME); 
    cv::setMouseCallback(WINDOW_NAME, mouse_callback);

    cv::Mat curFrame;
    rawVideo.getNextFrame(curFrame);
    imshow(WINDOW_NAME, curFrame);
    while (char c = cv::waitKey((int)(1000.0/frame_rate))) {
        if (c == ' ') pause = !pause; 
        if (c == 'q' || c == 'Q') return 0;
        if (!pause) {
            rawVideo.getNextFrame(curFrame);
        }
        imshow(WINDOW_NAME, curFrame);
    }
    return 0;
}
