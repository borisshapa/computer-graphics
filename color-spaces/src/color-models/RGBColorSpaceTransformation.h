//
// Created by borisshapa on 09.03.2021.
//

#ifndef COLOR_SPACES_RGBCOLORSPACETRANSFORMATION_H
#define COLOR_SPACES_RGBCOLORSPACETRANSFORMATION_H


#include <cstdint>
#include <array>
#include "ColorModel.h"

struct RGBColorSpaceTransformation : ColorModel {
    RGBColorSpaceTransformation(uint8_t y, uint8_t c1, uint8_t c2);

    RGB ToRGB() const override;

private:
    virtual std::array<std::array<double, 4>, 3> GetTransformationMatrix() const = 0;
};


#endif //COLOR_SPACES_RGBCOLORSPACETRANSFORMATION_H
