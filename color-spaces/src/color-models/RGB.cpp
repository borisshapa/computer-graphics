//
// Created by borisshapa on 07.03.2021.
//

#include "RGB.h"

RGB::RGB(uint8_t r, uint8_t g, uint8_t b) : ColorModel{r, g, b} {}

RGB RGB::ToRGB() const {
    return *this;
}
