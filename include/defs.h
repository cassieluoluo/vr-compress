#pragma once

typedef struct tagCompressedBlock {
    unsigned char block_type;
    unsigned char coeff[64];
    unsigned char padding[3];   // Memory alignment
} CompressedBlock;

typedef struct tagCmpHeader {
    unsigned short magic_number;
    unsigned short width;
    unsigned short height;
    unsigned short frame_rate;
    unsigned int num_frames;
} CmpHeader;



typedef struct tagBlockFrame {
    enum {
        BACKGROUND = 0x0,
        FOREGROUND = 0x1,
        COLOR_RED = 0x2,
        COLOR_GREEN = 0x4,
        COLOR_BLUE = 0x8,
    };
    unsigned int row;
    unsigned int col;
    unsigned char type;
    short coeff[64];
} BlockFrame;