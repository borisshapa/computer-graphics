//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_YCBCR601_H
#define COLOR_SPACES_YCBCR601_H

#include "YCbCr.h"

struct YCbCr601 : YCbCr {
    YCbCr601(uint8_t y, uint8_t cb, uint8_t cr);

    double GetKb() const override;

    double GetKr() const override;

    static const double KR;

    static const double KB;
};


#endif //COLOR_SPACES_YCBCR601_H
