//
// Created by borisshapa on 07.03.2021.
//

#include "YCbCr709.h"

YCbCr709::YCbCr709(uint8_t y, uint8_t cb, uint8_t cr) : YCbCr(y, cb, cr) {}

const double YCbCr709::KR = 0.2126;
const double YCbCr709::KB = 0.0722;

double YCbCr709::GetKb() const {
    return KB;
}

double YCbCr709::GetKr() const {
    return KR;
}
