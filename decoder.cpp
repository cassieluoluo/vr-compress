#include <vector>
#include <boost/filesystem.hpp>
#include "include/decoder.h"
#include <omp.h>

int Decoder::mouse_x;
int Decoder::mouse_y;

Decoder::Decoder(std::string filename, int w, int h, int fgstep, int bgstep, int roi)
    : width(w), height(h), foreground_step(fgstep), background_step(bgstep), roi_size(roi) {
    int file_size = boost::filesystem::file_size(filename);
    int blocks_per_frame = ((width - 1) / BLOCK_SIZE + 1) * ((height - 1) / BLOCK_SIZE + 1);    // TODO should use math::ceiling
    total_frames = file_size / (blocks_per_frame * sizeof(BlockFrame) * 3);
    infile = std::ifstream(filename, std::ios_base::binary);
    frame_count = 0;
}

cv::Mat Decoder::getNextFrame() {
    auto data = loadNextFrame();
    cv::Mat frame(cv::Size(width, height), CV_8UC3);

#pragma omp parallel for num_threads(8)
    for (int i = 0; i < data.size(); i += 3) {
        auto ch_red = block2Mat(data[i]);
        auto ch_green = block2Mat(data[i + 1]);
        auto ch_blue = block2Mat(data[i + 2]);
        std::vector<cv::Mat> channels{ch_blue, ch_green, ch_red};
        cv::Mat color_block(8, 8, CV_8UC3);
        cv::merge(channels, color_block);
        color_block.copyTo(frame(cv::Rect(data[i].col, data[i].row, BLOCK_SIZE, BLOCK_SIZE)));
    }

    return frame;
}

cv::Mat Decoder::block2Mat(BlockFrame& block) {
    // std::vector<short> data(block.coeff, block.coeff + 64);
    bool in_roi = block.row - mouse_y > -roi_size
               && block.row - mouse_y <  roi_size
               && block.col - mouse_x > -roi_size
               && block.col - mouse_x <  roi_size;
    if (in_roi) {
        if (debug_mode) return cv::Mat(BLOCK_SIZE, BLOCK_SIZE, CV_8UC1, cv::Scalar(255));
    } else if ((block.type & 1) == 0) {
        if (debug_mode) return cv::Mat(BLOCK_SIZE, BLOCK_SIZE, CV_8UC1, cv::Scalar(224));
        quantize(block.coeff, foreground_step);
    } else {
        if (debug_mode) return cv::Mat(BLOCK_SIZE, BLOCK_SIZE, CV_8UC1, cv::Scalar(32));
        quantize(block.coeff, background_step);
    }
    cv::Mat mb(64, 1, CV_16S, block.coeff);
    mb.convertTo(mb, CV_32FC1);
    mb = mb.reshape(0, BLOCK_SIZE);
    cv::idct(mb, mb);
    return mb;
}

std::vector<BlockFrame> Decoder::loadNextFrame() {
    const int BLOCK_SIZE = 8;
    if (frame_count == total_frames) {
        frame_count = 0;
        infile.seekg(SEEK_SET);
    }
    //Pre-assign
    std::vector<BlockFrame> frame(height / BLOCK_SIZE * width / BLOCK_SIZE * 3);
    int count = 0;
    for (int row = 0; row < height; row += BLOCK_SIZE) {
        for (int col = 0; col < width; col += BLOCK_SIZE) {
            for (int ch = 0; ch < 3; ch++) {
                BlockFrame block;
                infile.read(reinterpret_cast<char *>(&block),
                    sizeof(BlockFrame));
                frame[count++] = block;
            }
        }
    }
    frame_count++;
    return frame;
}

