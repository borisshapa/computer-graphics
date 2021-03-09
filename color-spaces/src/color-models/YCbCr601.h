//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_YCBCR601_H
#define COLOR_SPACES_YCBCR601_H

#include "RGBColorSpaceTransformation.h"

struct YCbCr601: RGBColorSpaceTransformation {
    YCbCr601(uint8_t y, uint8_t cb, uint8_t cr);

    uint8_t Y() const;

    uint8_t Cb() const;

    uint8_t Cr() const;

private:
    std::array<std::array<double, 4>, 3> GetTransformationMatrix() const override;
};


#endif //COLOR_SPACES_YCBCR601_H
