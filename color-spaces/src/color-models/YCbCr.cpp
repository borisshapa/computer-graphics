//
// Created by borisshapa on 11.03.2021.
//

#include "YCbCr.h"
#include "RGB.h"

YCbCr::YCbCr(uint8_t y, uint8_t cb, uint8_t cr) : ColorModel{y, cb, cr} {}

double YCbCr::GetKg() const {
    return 1.0 - GetKb() - GetKr();
}

RGB YCbCr::ToRGB() const {
    std::array<double, 3> yPbPr = {(GetY() - 16) / 219.0, (GetCb() - 128) / 224.0, (GetCr() - 128) / 224.0};

    std::array<std::array<double, 3>, 3> transformation_matrix = {{
                                                                          {1.0, 0.0, 2 - 2 * GetKr()},
                                                                          {1.0, -(GetKb() / GetKg()) *
                                                                                (2 - 2 * GetKb()),
                                                                                  -(GetKr() / GetKg()) *
                                                                                  (2 - 2 * GetKr())},
                                                                          {1.0, 2 - 2 * GetKb(), 0.0}
                                                                  }};

    std::array<double, 3> rgb = ColorModel::Transform(transformation_matrix, yPbPr);

    return RGB(GetByteValueByZeroOneRangeValue(rgb[0]),
               GetByteValueByZeroOneRangeValue(rgb[1]),
               GetByteValueByZeroOneRangeValue(rgb[2]));
}

uint8_t YCbCr::GetY() const {
    return GetChannelValues()[0];
}

uint8_t YCbCr::GetCb() const {
    return GetChannelValues()[1];
}

uint8_t YCbCr::GetCr() const {
    return GetChannelValues()[2];
}

double YCbCr::GetKb() const {
    return 0.0;
}

double YCbCr::GetKr() const {
    return 0.0;
}

