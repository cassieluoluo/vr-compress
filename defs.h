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
