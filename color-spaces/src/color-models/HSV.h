//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_HSV_H
#define COLOR_SPACES_HSV_H


#include "ColorModel.h"

struct HSV : ColorModel {
    HSV(uint8_t h, uint8_t s, uint8_t v);

    RGB ToRGB() override;

    uint8_t H() const;

    uint8_t S() const;

    uint8_t V() const;
};


#endif //COLOR_SPACES_HSV_H
