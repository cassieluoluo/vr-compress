#include <iostream>
#include <cstring>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <boost/program_options.hpp>
#include "include/utils.h"
#include "include/defs.h"

namespace po = boost::program_options;
using namespace cv;
using namespace std;

#include "include/raw_video.h"

void writeToFile(ofstream &fo, BlockFrame &bf, short *buf) {
    memcpy(&bf.coeff, buf, 64 * sizeof(short));
    fo.write((const char *)&bf, sizeof(BlockFrame));
}

int main(int argc, char **argv) {
    int width, height;
    std::string filename;
    int motionThreshold;
    try {
        po::options_description desc("A basic video player that plays raw rgb video");
        desc.add_options()
            ("help", "display help message")
            ("width", po::value<int>(&width)->default_value(960), "the width of the video frame")
            ("height",po::value<int>(&height)->default_value(540), "the height of the video frame")
            ("input-file", po::value<std::string>(), "input video file")
            ("threshold,t", po::value<int>(&motionThreshold)->default_value(128*40), "threshold for motion vector");
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
    ofstream fout(filename + ".cmp", ios_base::binary);
    cv::namedWindow("fgMaskMOG");
    cv::Mat curFrame;
    cv::Mat fgMaskMOG;
    Ptr<BackgroundSubtractor> pMOG;
    pMOG = cv::createBackgroundSubtractorMOG2(100, 50);
    int counter = 0;
    while (char c = cv::waitKey(1)) {
        if (c == 'q') break;
        if (rawVideo.isLastFrame()) break;
        rawVideo.getNextFrame(curFrame);
        pMOG->apply(curFrame, fgMaskMOG);
        imshow("fgMaskMOG", fgMaskMOG);
        vector<Mat> blocks;
        Utils::divideImg8by8(fgMaskMOG, blocks);
        vector<int> motions;
        Utils::calculateBlocksSum(blocks, motions);
        Utils::divideImg8by8(curFrame, blocks);
        auto it = motions.begin();
        auto bit = blocks.begin();
        for (unsigned int row = 0; row < fgMaskMOG.size().height; row += 8) {
            for (unsigned int col = 0; col < fgMaskMOG.size().width; col += 8) {
                BlockFrame blockFrame;

                blockFrame.col = col;
                blockFrame.row = row;
                if (*it.base() > motionThreshold) {
                    blockFrame.type = BlockFrame::BACKGROUND;
                } else {
                    blockFrame.type = BlockFrame::FOREGROUND;
                }
                Mat buf;
                Mat dct;

                blockFrame.type &= 1;
                blockFrame.type |= BlockFrame::COLOR_RED;
                Utils::getColorFromBlock(*bit.base(), buf, Utils::COLOR_RED);
                Utils::calculateBlockDCT(buf, dct);
                writeToFile(fout, blockFrame, dct.ptr<short>(0));

                blockFrame.type &= 1;
                blockFrame.type |= BlockFrame::COLOR_GREEN;
                Utils::getColorFromBlock(*bit.base(), buf, Utils::COLOR_GREEN);
                Utils::calculateBlockDCT(buf, dct);
                writeToFile(fout, blockFrame, dct.ptr<short>(0));

                blockFrame.type &= 1;
                blockFrame.type |= BlockFrame::COLOR_BLUE;
                Utils::getColorFromBlock(*bit.base(), buf, Utils::COLOR_BLUE);
                Utils::calculateBlockDCT(buf, dct);
                writeToFile(fout, blockFrame, dct.ptr<short>(0));

                it++;
                bit++;
            }
        }
        cout << counter++ << " frames" << endl;
    }
    fout.close();
    destroyAllWindows();
    return 0;
}
