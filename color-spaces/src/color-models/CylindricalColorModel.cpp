//
// Created by borisshapa on 09.03.2021.
//

#include <cassert>
#include "CylindricalColorModel.h"

double FindOutValue(uint8_t sector, const double rgb_max, const double rgb_min, const double inc_val,
                    const double dec_val) {
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

RGB CylindricalColorModel::ToRGB(const double hue, const double rgb_min_value, const double rgb_max_value) {
    const uint8_t sector = hue / 60;
    const double color_increasing_value_quotient = (hue - sector * 60) / 60;
    const double color_decreasing_value_quotient = 1.0 - color_increasing_value_quotient;

    const double range = rgb_max_value - rgb_min_value;
    const double color_increasing_value = color_increasing_value_quotient * range + rgb_min_value;
    const double color_decreasing_value = color_decreasing_value_quotient * range + rgb_min_value;

    const double red = FindOutValue(sector, rgb_max_value, rgb_min_value, color_increasing_value,
                                    color_decreasing_value);
    const double green = FindOutValue((sector + 4) % 6, rgb_max_value, rgb_min_value, color_increasing_value,
                                      color_decreasing_value);
    const double blue = FindOutValue((sector + 2) % 6, rgb_max_value, rgb_min_value, color_increasing_value,
                                     color_decreasing_value);
    return RGB(static_cast<uint8_t>(red * 255),
               static_cast<uint8_t>(green * 255),
               static_cast<uint8_t>(blue * 255));
}

CylindricalColorModel::CylindricalColorModel(uint8_t h, uint8_t s, uint8_t x) : ColorModel{h, s, x} {}
