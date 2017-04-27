#include <fstream>
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace cv;
using namespace std;

#include "include/raw_video.h"

int main(int argc, char **argv) {
    int width, height;
    std::string filename;
    try { 
        po::options_description desc("A basic video player that plays raw rgb video");
        desc.add_options()
            ("help", "display help message")
            ("width", po::value<int>(&width)->default_value(960), "the width of the video frame")
            ("height",po::value<int>(&height)->default_value(540), "the height of the video frame")
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
    namedWindow("Basic Video Player");
    namedWindow("fgMask");
    Mat fgMaskMOG2;
    Mat curFrame;
    Ptr<BackgroundSubtractor> pMOG2;
    pMOG2 = cv::createBackgroundSubtractorMOG2(30);
    cout<<123<<endl;
    while(!rawVideo.isLastFrame()) {
    cout<<456<<endl;
        rawVideo.getNextFrame(curFrame);
        pMOG2->apply(curFrame, fgMaskMOG2);
        imshow("Basic Video Player", curFrame);
        imshow("fgMask", fgMaskMOG2);
    }
    destroyAllWindows();
    return 0;
}