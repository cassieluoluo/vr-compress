#include <algorithm>
#include "include/cmp_block.h"

CompressedBlock::CompressedBlock(std::vector<short>& c, bool type) {
    coeffs = c;
    block_type = type;
}

std::vector<short> CompressedBlock::bytes() {
    return coeffs;
}

unsigned char CompressedBlock::type() {
    return block_type;
}

cv::Mat CompressedBlock::decode(int step, bool gazing) {
    cv::Mat block;
    if (!gazing) {
        quantize(step);
    }
    cv::idct(block, block);
    return block;
}

cv::Mat CompressedBlock::quantize(int step) {
    std::vector<short> quantized(coeffs.size(), 0);
    std::transform(coeffs.begin(), coeffs.end(), quantized.begin(),
        [step](short val) { return (val / step) * step; });
    cv::Mat result(quantized); // need to refactor the way of quantization
    return result;
}