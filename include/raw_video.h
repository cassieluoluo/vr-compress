/**
 * raw_video.h
 * 
 * Author: Luoyili Zhou (luoyiliz@usc.edu)
 * 
 * Definition of RawVideo class
 */

#pragma once
#include <fstream>
#include <queue>
#include <opencv2/opencv.hpp>

enum ColorSpace {
    CS_RGB,
    CS_YUV
};

class RawVideo {
public:
    RawVideo(std::string filename, int width, int height, double frame_rate = 25.0);
    ~RawVideo();
    cv::Mat getFrame(int frame_id, ColorSpace color_space = CS_RGB);
    cv::Mat getFrame(std::string timestamp, ColorSpace color_space = CS_RGB);
    cv::Mat getNextFrame(ColorSpace color_space = CS_RGB);
    void resetCounter();

private:
    static const int PRELOAD = 25;
    bool _loop;
    int _current_frame;
    int _total_frames;
    int _width;
    int _height;
    int _frame_size;
    std::ifstream _input_file;
    std::queue<cv::Mat> _buffer;    // possible optimization: use rotational array instead of queue
    unsigned char *_frame_buffer;
    cv::Mat loadFrame(); // Load next frame based on _current_frame
};
