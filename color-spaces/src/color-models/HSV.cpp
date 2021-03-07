//
// Created by borisshapa on 07.03.2021.
//

#include <cassert>
#include "HSV.h"

HSV::HSV(uint8_t h, uint8_t s, uint8_t v) : ColorModel{h, s, v} {}

double FindOutValue(uint8_t sector, double rgb_max, double rgb_min, double inc_val, double dec_val) {
    sector = sector % 6;
    switch (sector) {
        case 0:
            return rgb_max;
        case 1:
            return dec_val;
        case 2:
            return rgb_min;
        case 3:
            return rgb_min;
        case 4:
            return inc_val;
        case 5:
            return rgb_max;
        default:
            assert("The maximum value is 5");
    }
}

RGB HSV::ToRGB() {
    double hue = (static_cast<double>(H()) / 255) * 360;
    double saturation = static_cast<double>(S()) / 255;
    double value = static_cast<double>(V()) / 255;

    double range = value * saturation;
    double rgb_max_value = value;
    double rgb_min_value = rgb_max_value - range;
    uint8_t sector = hue / 60;
    double color_increasing_value_quotient = (hue - sector * 60) / 60;
    double color_decreasing_value_quotient = 1.0 - color_increasing_value_quotient;

    double color_increasing_value = color_increasing_value_quotient * range + rgb_min_value;
    double color_decreasing_value = color_decreasing_value_quotient * range + rgb_min_value;

    double red = FindOutValue(sector, rgb_max_value, rgb_min_value, color_increasing_value, color_decreasing_value);
    double green = FindOutValue((sector + 4) % 6, rgb_max_value, rgb_min_value, color_increasing_value,
                                color_decreasing_value);
    double blue = FindOutValue((sector + 2) % 6, rgb_max_value, rgb_min_value, color_increasing_value,
                               color_decreasing_value);
    return RGB(static_cast<uint8_t>(red * 255),
               static_cast<uint8_t>(green * 255),
               static_cast<uint8_t>(blue * 255));
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
