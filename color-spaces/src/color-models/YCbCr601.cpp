//
// Created by borisshapa on 07.03.2021.
//

#include "YCbCr601.h"

YCbCr601::YCbCr601(uint8_t y, uint8_t cb, uint8_t cr) : YCbCr(y, cb, cr) {}

const double YCbCr601::KR = 0.299;
const double YCbCr601::KB = 0.114;

double YCbCr601::GetKb() const {
    return KB;
}

double YCbCr601::GetKr() const {
    return KR;
}
