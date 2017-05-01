/**
 * decoder.h
 *
 * Author: Luoyili Zhou (luoyiliz@usc.edu)
 *
 */

#pragma once
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include "defs.h"
class Decoder {
public:
    Decoder(std::string filename, int width, int height, int fgstep, int bgstep);
    cv::Mat getNextFrame();
    void toggleDebugMode(bool debug) { debug_mode = debug; }
    static void setMousePosition(int event, int x, int y, int flag, void *param);
    static const int BLOCK_SIZE = 8;
    static const int ROI_SIZE = 32;
    static int mouse_x, mouse_y;
private:
    std::ifstream infile;
    int width, height;
    int foreground_step, background_step;
    bool debug_mode;
    int frame_count, total_frames;
    cv::Mat block2Mat(BlockFrame block);
    std::vector<BlockFrame> loadNextFrame();
    void quantize(std::vector<short>& data, int step) {
        std::transform(data.begin(), data.end(), data.begin(),
            [step](short val) { return (val / step) * step; });
    }
};