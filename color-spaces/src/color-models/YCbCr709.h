//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_YCBCR709_H
#define COLOR_SPACES_YCBCR709_H

#include "YCbCr.h"

struct YCbCr709 : YCbCr {
    YCbCr709(uint8_t y, uint8_t cb, uint8_t cr);

    double GetKb() const override;

    double GetKr() const override;

    static const double KB;
    static const double KR;
};


#endif //COLOR_SPACES_YCBCR709_H
