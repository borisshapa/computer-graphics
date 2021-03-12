//
// Created by borisshapa on 07.03.2021.
//

#include "ColorModel.h"
#include "HSL.h"
#include "RGB.h"

RGB HSL::ToRGB() const {
    const double hue = (static_cast<double>(H()) / 255) * 360;
    const double saturation = static_cast<double>(S()) / 255;
    const double lightness = static_cast<double>(L()) / 255;

    const double half_range = (lightness < 0.5) ? lightness * saturation : (1 - lightness) * saturation;
    const double rgb_max_value = lightness + half_range;
    const double rgb_min_value = lightness - half_range;

    return CylindricalColorModel::ToRGB(hue, rgb_min_value, rgb_max_value);
}

HSL::HSL(uint8_t h, uint8_t s, uint8_t l) : CylindricalColorModel(h, s, l) {}

uint8_t HSL::H() const {
    return GetChannelValues()[0];
}

uint8_t HSL::S() const {
    return GetChannelValues()[1];
}

uint8_t HSL::L() const {
    return GetChannelValues()[2];
}
