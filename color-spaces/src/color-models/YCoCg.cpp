//
// Created by borisshapa on 07.03.2021.
//

#include "YCoCg.h"
#include "RGB.h"

YCoCg::YCoCg(uint8_t y, uint8_t co, uint8_t cg) : ColorModel{y, co, cg} {}

uint8_t YCoCg::Y() const {
    return GetChannelValues()[0];
}

uint8_t YCoCg::Co() const {
    return GetChannelValues()[1];
}

uint8_t YCoCg::Cg() const {
    return GetChannelValues()[2];
}

RGB YCoCg::ToRGB() const {
    std::array<double, 3> yCoCg = {Y() / 255.0, (Co() / 255.0) - 0.5, (Cg() / 255.0) - 0.5};
    std::array<std::array<double, 3>, 3> transformation_matrix = {{
                                                                          {1.0, 1.0, -1.0},
                                                                          {1.0, 0.0, 1.0},
                                                                          {1.0, -1.0, -1.0}
                                                                  }};
    std::array<double, 3> rgb = ColorModel::Transform(transformation_matrix, yCoCg);

    return RGB(GetByteValueByZeroOneRangeValue(rgb[0]),
               GetByteValueByZeroOneRangeValue(rgb[1]),
               GetByteValueByZeroOneRangeValue(rgb[2]));
}
