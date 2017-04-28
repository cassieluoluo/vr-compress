/**
 * raw_video.cpp
 * 
 * Author: Luoyili Zhou (luoyiliz@usc.edu)
 * 
 * Implementation of RawVideo class.
 * Note
 *  - The default storage format for this project is flat RGB. The channels are
 *    stored in RRR...RGGG...GBBB...B
 */

#include <exception>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#include "include/raw_video.h"

RawVideo::RawVideo(
    std::string filename,
    int width,
    int height,
    double frame_rate
) {
    int file_size = boost::filesystem::file_size(filename);
    _input_file = std::ifstream(filename, std::ios_base::binary);
    _width = width;
    _height = height;
    _frame_size = _width*_height;
    _total_frames = file_size / (_frame_size * 3);
    _current_frame = 0;
    _frame_buffer = new unsigned char[_frame_size*3];
}

RawVideo::~RawVideo() {
    delete _frame_buffer;
}

cv::Mat RawVideo::getFrame(int frame_id, ColorSpace color_space) {
    return cv::Mat();
}

cv::Mat RawVideo::getFrame(std::string timestamp, ColorSpace color_space) {
    return cv::Mat();
}

void RawVideo::getNextFrame(cv::Mat& frame) {
    if (_current_frame == _total_frames) {
        resetCounter();
    }

    loadFrame(frame);
    _current_frame++;
    return;

}

void RawVideo::resetCounter() {
    _current_frame = 0;
    _input_file.seekg(SEEK_SET);
}

bool RawVideo::isLastFrame() {
    return _current_frame == _total_frames;
}

void RawVideo::loadFrame(cv::Mat& frame) {
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < _frame_size; i++) {
            _input_file.read((char *)_frame_buffer + i*3 + c, 1);
        }
    }
    frame = cv::Mat(cv::Size(_width, _height), CV_8UC3, _frame_buffer);
    cv::cvtColor(frame, frame, CV_RGB2BGR);
    return;
}