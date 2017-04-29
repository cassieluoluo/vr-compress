#include "include/cmp_block.h"

std::vector<unsigned char>& CompressedBlock::bytes() {
    return coeffs;
}

unsigned char CompressedBlock::type() {
    return block_type;
}

cv::Mat CompressedBlock::decode(int q_level) {
    // decode the block based on the input quantization level
    return cv::Mat();
}
