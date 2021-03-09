//
// Created by borisshapa on 07.03.2021.
//

#include "YCbCr709.h"

YCbCr709::YCbCr709(uint8_t y, uint8_t cb, uint8_t cr) : RGBColorSpaceTransformation(y, cb, cr) {}

uint8_t YCbCr709::Y() const {
    return GetChannelValues()[0];
}

uint8_t YCbCr709::Cb() const {
    return GetChannelValues()[1];
}

uint8_t YCbCr709::Cr() const {
    return GetChannelValues()[2];
}

std::array<std::array<double, 4>, 3> YCbCr709::GetTransformationMatrix() const {
    return {{
                    {1.0, 0.0, 1.042, -133.376},
                    {1.0, -0.34414, -0.71414, 135.45984},
                    {1.0, 1.772, 0.0, -226.816}
            }};
}
