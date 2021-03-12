//
// Created by borisshapa on 07.03.2021.
//

#include <iostream>
#include "CMY.h"
#include "RGB.h"

CMY::CMY(uint8_t c, uint8_t m, uint8_t y) : ColorModel{c, m, y} {}

RGB CMY::ToRGB() const {
    return RGB(255 - C(), 255 - M(), 255 - Y());
}

uint8_t CMY::C() const {
    return GetChannelValues()[0];
}

uint8_t CMY::M() const {
    return GetChannelValues()[1];
}

uint8_t CMY::Y() const {
    return GetChannelValues()[2];
}
