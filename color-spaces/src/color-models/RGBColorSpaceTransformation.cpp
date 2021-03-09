//
// Created by borisshapa on 09.03.2021.
//

#include <cstddef>
#include <algorithm>
#include "RGBColorSpaceTransformation.h"

RGBColorSpaceTransformation::RGBColorSpaceTransformation(uint8_t y, uint8_t c1, uint8_t c2) : ColorModel{y, c1, c2} {}

RGB RGBColorSpaceTransformation::ToRGB() const {
    auto transformation_matrix = GetTransformationMatrix();

    double rgb[3];
    uint8_t ycc[4];
    std::copy(GetChannelValues(), GetChannelValues() + 3, ycc);
    ycc[3] = 1;

    for (size_t i = 0; i < transformation_matrix.size(); i++) {
        rgb[i] = 0;
        for (size_t j = 0; j < transformation_matrix[i].size(); j++) {
            rgb[i] = transformation_matrix[i][j] * ycc[j];
        }
    }
    for (double &channel : rgb) {
        channel = std::max(0., std::min(channel, 255.));
    }
    return RGB(static_cast<uint8_t>(rgb[0]),
               static_cast<uint8_t>(rgb[1]),
               static_cast<uint8_t>(rgb[2]));
}