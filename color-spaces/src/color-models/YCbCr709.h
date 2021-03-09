//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_YCBCR709_H
#define COLOR_SPACES_YCBCR709_H


#include "RGBColorSpaceTransformation.h"

struct YCbCr709 : RGBColorSpaceTransformation {
    YCbCr709(uint8_t y, uint8_t cb, uint8_t cr);

    uint8_t Y() const;

    uint8_t Cb() const;

    uint8_t Cr() const;

private:
    std::array<std::array<double, 4>, 3> GetTransformationMatrix() const override;
};


#endif //COLOR_SPACES_YCBCR709_H
