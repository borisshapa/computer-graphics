//
// Created by borisshapa on 07.03.2021.
//

#include <cassert>
#include "HSV.h"

HSV::HSV(uint8_t h, uint8_t s, uint8_t v) : CylindricalColorModel(h, s, v) {}

RGB HSV::ToRGB() const {
    const double hue = (static_cast<double>(H()) / 255) * 360;
    const double saturation = static_cast<double>(S()) / 255;
    const double value = static_cast<double>(V()) / 255;

    const double range = value * saturation;
    const double rgb_max_value = value;
    const double rgb_min_value = rgb_max_value - range;

    return CylindricalColorModel::ToRGB(hue, rgb_min_value, rgb_max_value);
}

uint8_t HSV::H() const {
    return GetChannelValues()[0];
}

uint8_t HSV::S() const {
    return GetChannelValues()[1];
}

uint8_t HSV::V() const {
    return GetChannelValues()[2];
}
