#include <vector>
#include "include/decoder.h"

int Decoder::mouse_x;
int Decoder::mouse_y;

Decoder::Decoder(std::string filename, int w, int h, int fgstep, int bgstep)
    : width(w), height(h), foreground_step(fgstep), background_step(bgstep) {
    infile = std::ifstream(filename, std::ios_base::binary);
}

cv::Mat Decoder::getNextFrame() {
    auto data = loadNextFrame();
    cv::Mat frame(cv::Size(width, height), CV_8UC3);
    cv::Mat color_block(8, 8, CV_8UC3);
    for (int i = 0; i < data.size(); i += 3) {
        auto ch_red = block2Mat(data[i]);
        auto ch_green = block2Mat(data[i + 1]);
        auto ch_blue = block2Mat(data[i + 2]);
        std::vector<cv::Mat> channels{ch_red, ch_green, ch_blue};
        cv::merge(channels, color_block);
        color_block.copyTo(frame(cv::Rect(data[i].col, data[i].row, 8, 8)));
    }
    return frame;
}

cv::Mat Decoder::block2Mat(BlockFrame block) {
    std::vector<short> data(block.coeff, block.coeff + 64);
    bool in_roi = block.row - mouse_y > -32
               && block.row - mouse_y < 32
               && block.col - mouse_x > -32
               && block.col - mouse_x < 32;
    if (in_roi) {
        if (debug_mode) return cv::Mat(8, 8, CV_8UC1, cv::Scalar(255));
    }else if (block.type % 2 == 0) {
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

std::vector<BlockFrame> Decoder::loadNextFrame() {
    const int BLOCK_SIZE = 8;
    std::vector<BlockFrame> frame;
    for (int row = 0; row < height; row += BLOCK_SIZE) {
        for (int col = 0; col < width; col += BLOCK_SIZE) {
            for (int ch = 0; ch < 3; ch++) {
                BlockFrame block;
                infile.read(reinterpret_cast<char *>(&block),
                    sizeof(BlockFrame));
                frame.push_back(block);
            }
        }
    }
    return frame;
}

void Decoder::setMousePosition(int event, int x, int y, int flag, void *param) {
    mouse_x = x;
    mouse_y = y;
}
