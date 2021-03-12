//
// Created by borisshapa on 11.03.2021.
//

#ifndef COLOR_SPACES_YCBCR_H
#define COLOR_SPACES_YCBCR_H

#include "ColorModel.h"

struct YCbCr : ColorModel {
    YCbCr(uint8_t y, uint8_t cb, uint8_t cr);

    uint8_t GetY() const;

    uint8_t GetCb() const;

    uint8_t GetCr() const;

    virtual double GetKb() const;

    virtual double GetKr() const;

    double GetKg() const;

    RGB ToRGB() const override;
};


#endif //COLOR_SPACES_YCBCR_H
