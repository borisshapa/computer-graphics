//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_HSV_H
#define COLOR_SPACES_HSV_H


#include "ColorModel.h"
#include "CylindricalColorModel.h"

struct HSV : CylindricalColorModel {
    HSV(uint8_t h, uint8_t s, uint8_t v);

    RGB ToRGB() const override;

    uint8_t H() const;

    uint8_t S() const;

    uint8_t V() const;
};


#endif //COLOR_SPACES_HSV_H
