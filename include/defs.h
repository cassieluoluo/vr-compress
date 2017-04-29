#pragma once

typedef struct tagCmpHeader {
    unsigned short magic_number;
    unsigned short width;
    unsigned short height;
    unsigned short frame_rate;
    unsigned int num_frames;
} CmpHeader;
