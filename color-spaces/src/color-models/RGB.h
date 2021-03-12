//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_RGB_H
#define COLOR_SPACES_RGB_H

#include "ColorModel.h"
#include "CMY.h"
#include "HSL.h"
#include "HSV.h"
#include "YCbCr601.h"
#include "YCbCr709.h"
#include "YCoCg.h"

struct RGB : ColorModel {
    RGB(uint8_t r, uint8_t g, uint8_t b);

    RGB ToRGB() const override;

    uint8_t R() const;

    uint8_t G() const;

    uint8_t B() const;

    CMY ToCMY() const;

    HSL ToHSL() const;

    HSV ToHSV() const;

    YCbCr601 ToYCbCr601() const;

    YCbCr709 ToYCbCr709() const;

    YCoCg ToYCoCg() const;
};


#endif //COLOR_SPACES_RGB_H
