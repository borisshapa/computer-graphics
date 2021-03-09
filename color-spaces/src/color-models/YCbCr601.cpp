//
// Created by borisshapa on 07.03.2021.
//

#include "YCbCr601.h"

YCbCr601::YCbCr601(uint8_t y, uint8_t cb, uint8_t cr) : RGBColorSpaceTransformation(y, cb, cr) {}

uint8_t YCbCr601::Y() const {
    return GetChannelValues()[0];
}

uint8_t YCbCr601::Cb() const {
    return GetChannelValues()[1];
}

uint8_t YCbCr601::Cr() const {
    return GetChannelValues()[2];
}

std::array<std::array<double, 4>, 3> YCbCr601::GetTransformationMatrix() const {
    return {{
                    {1.164382813, 0.0, 1.596027344, -222.921},
                    {1.164382813, -0.391761719, -0.81296875, 135.576},
                    {1.164382813, 2.017234375, 0.0, -276.836}
            }};
}
