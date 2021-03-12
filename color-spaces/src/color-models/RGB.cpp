//
// Created by borisshapa on 07.03.2021.
//

#include <algorithm>
#include <tuple>
#include <cmath>
#include "RGB.h"

RGB::RGB(uint8_t r, uint8_t g, uint8_t b) : ColorModel{r, g, b} {}

RGB RGB::ToRGB() const {
    return *this;
}

uint8_t RGB::R() const {
    return GetChannelValues()[0];
}

uint8_t RGB::G() const {
    return GetChannelValues()[1];
}

uint8_t RGB::B() const {
    return GetChannelValues()[2];
}

CMY RGB::ToCMY() const {
    return CMY(255 - R(), 255 - G(), 255 - B());
}

std::tuple<double, double, double> GetHMaxMin(uint8_t red, uint8_t green, uint8_t blue) {
    const double r = red / 255.0;
    const double g = green / 255.0;
    const double b = blue / 255.0;
    const double max = std::max(r, std::max(g, b));
    const double min = std::min(r, std::min(g, b));
    const double range = max - min;
    double h;
    if (range == 0) {
        h = 0.0;
    } else if (max == r) {
        h = 60 * (std::fmod((g - b) / range, 6));
    } else if (max == g) {
        h = 60 * (2 + ((b - r) / range));
    } else {
        h = 60 * (4 + ((r - g) / range));
    }
    if (h < 0) {
        h += 360;
    }
    return std::make_tuple(h, max, min);
}

HSL RGB::ToHSL() const {
    double h, max, min;
    std::tie(h, max, min) = GetHMaxMin(R(), G(), B());
    double range = max - min;
    const double l = (max + min) / 2;
    const double s = range == 0 ? 0.0 : range / (1 - std::abs(2 * l - 1));
    return HSL(GetByteValueByZeroOneRangeValue(h / 360.0),
               GetByteValueByZeroOneRangeValue(s),
               GetByteValueByZeroOneRangeValue(l));
}

HSV RGB::ToHSV() const {
    double h, max, min;
    std::tie(h, max, min) = GetHMaxMin(R(), G(), B());
    const double s = (max == 0) ? 0.0 : (max - min) / max;
    const double v = max;
    return HSV(GetByteValueByZeroOneRangeValue(h / 360.0),
               GetByteValueByZeroOneRangeValue(s),
               GetByteValueByZeroOneRangeValue(v));

}

std::tuple<uint8_t, uint8_t, uint8_t> RGB2YCbCr(uint8_t r, uint8_t g, uint8_t b, double kr, double kg, double kb) {
    const std::array<double, 3> rgb = {r / 255.0, g / 255.0, b / 255.0};
    const std::array<std::array<double, 3>, 3> transformation_matrix = {{
                                                                                {kr, kg, kb},
                                                                                {-0.5 * (kr / (1 - kb)),
                                                                                        -0.5 * (kg / (1 - kb)), 0.5},
                                                                                {0.5, -0.5 * (kg / (1 - kr)),
                                                                                        -0.5 * (kb / (1 - kr))}
                                                                        }};

    std::array<double, 3> yPbPr = ColorModel::Transform(transformation_matrix, rgb);

    return std::make_tuple(ColorModel::ClampDouble(16 + 219 * yPbPr[0]),
                           ColorModel::ClampDouble(128 + 224 * yPbPr[1]),
                           ColorModel::ClampDouble(128 + 224 * yPbPr[2]));
}

YCbCr601 RGB::ToYCbCr601() const {
    double y, cb, cr;
    std::tie(y, cb, cr) = RGB2YCbCr(R(), G(), B(), YCbCr601::KR, 1 - YCbCr601::KR - YCbCr601::KB, YCbCr601::KB);
    return YCbCr601(y, cb, cr);
}

YCbCr709 RGB::ToYCbCr709() const {
    double y, cb, cr;
    std::tie(y, cb, cr) = RGB2YCbCr(R(), G(), B(), YCbCr709::KR, 1 - YCbCr709::KR - YCbCr709::KB, YCbCr709::KB);
    return YCbCr709(y, cb, cr);
}

YCoCg RGB::ToYCoCg() const {
    std::array<double, 3> rgb = {R() / 255.0, G() / 255.0, B() / 255.0};
    std::array<std::array<double, 3>, 3> transformation_matrix = {{
                                                                          {0.25, 0.5, 0.25},
                                                                          {0.5, 0, -0.5},
                                                                          {-0.25, 0.5, -0.25}
                                                                  }};
    std::array<double, 3> yCoCg = ColorModel::Transform(transformation_matrix, rgb);
    return YCoCg(GetByteValueByZeroOneRangeValue(yCoCg[0]),
                 GetByteValueByZeroOneRangeValue(yCoCg[1] + 0.5),
                 GetByteValueByZeroOneRangeValue(yCoCg[2] + 0.5));
}
