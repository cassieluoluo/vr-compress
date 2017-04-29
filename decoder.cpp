/**
 * decoder.cpp
 *
 * The decoder
 *
 * Author: Luoyili Zhou (luoyiliz@usc.edu)
 *
 */

#include <opencv2/opencv.hpp>

#include "include/defs.h"
#include "include/cmp_file.h"

int main(int argc, char **argv) {
    CmpFile cmpfile("filename.cmp");
    while (cmpfile.hasNextFrame()) {
        auto frame = cmpfile.getNextFrame();
        cv::Mat 
    }
}