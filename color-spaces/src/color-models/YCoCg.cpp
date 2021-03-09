//
// Created by borisshapa on 07.03.2021.
//

#include "YCoCg.h"

YCoCg::YCoCg(uint8_t y, uint8_t co, uint8_t cg) : RGBColorSpaceTransformation(y, co, cg) {}

uint8_t YCoCg::Y() const {
    return GetChannelValues()[0];
}

uint8_t YCoCg::Co() const {
    return GetChannelValues()[1];
}

uint8_t YCoCg::Cg() const {
    return GetChannelValues()[2];
}

std::array<std::array<double, 4>, 3> YCoCg::GetTransformationMatrix() const {
    return {{
                    {1.0, 1.0, -1.0, 0.0},
                    {1.0, 0.0, 1.0, 0.0},
                    {1.0, -1.0, -1.0, 0.0}
            }};
}
