#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <boost/program_options.hpp>
#include "include/utils.h"

namespace po = boost::program_options;
using namespace cv;
using namespace std;

int motionThreshold = 128*40;

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
//    cv::namedWindow("Basic Video Player");
    cv::namedWindow("fgMaskMOG");
    cv::namedWindow("motions");
    cv::Mat curFrame;
    cv::Mat fgMaskMOG;
    Ptr<BackgroundSubtractor> pMOG;
    pMOG = cv::createBackgroundSubtractorMOG2(100, 50);
    // imshow("Basic Video Player", curFrame);
    while (char c = cv::waitKey(1)) {
        if (c == 'q' || c == 'Q') break;
        if (rawVideo.isLastFrame()) rawVideo.resetCounter();
        rawVideo.getNextFrame(curFrame);
//        imshow("Basic Video Player", curFrame);
        pMOG->apply(curFrame, fgMaskMOG);
        imshow("fgMaskMOG", fgMaskMOG);
        vector<Mat> blocks;
        Utils::divideImg8by8(fgMaskMOG, blocks);
        vector<int> motions;
        Utils::calculateBlockSum(blocks, motions);
        Mat res;
        curFrame.copyTo(res);
        auto it = motions.begin();
        auto bit = blocks.begin();
        for (int row = 0; row < fgMaskMOG.size().height; row += 8) {
            for (int col = 0; col < fgMaskMOG.size().width; col += 8) {
                if (*it.base() > motionThreshold) {
                    for (int i = 0; i < 4; ++i) {
                        for (int j = 0; j < 8; ++j) {
                            res.at<Vec3b>(row + i, col + j)[2] = 255;
                        }
                    }
                }
                it++;
                bit++;
            }
        }
        imshow("motions", res);

    }
    destroyAllWindows();
    return 0;
}
