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

cv::Mat RawVideo::getNextFrame(ColorSpace color_space) {
    if (_current_frame == _total_frames) {
        resetCounter();
    }
    cv::Mat frame = loadFrame();
    if (color_space == CS_YUV) {
        cv::cvtColor(frame, frame, CV_RGB2YUV);
    }
    return frame;
}

void RawVideo::resetCounter() {
    _current_frame = 0;
}

cv::Mat RawVideo::loadFrame() {
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < _frame_size; i++) {
            _input_file.read((char *)_frame_buffer + i*3 + c, 1);
        }
    }
    cv::Mat frame(cv::Size(_width, _height), CV_8UC3, _frame_buffer);
    cv::cvtColor(frame, frame, CV_RGB2BGR);
    return frame;
}