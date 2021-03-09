//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_HSL_H
#define COLOR_SPACES_HSL_H

#include "ColorModel.h"
#include "CylindricalColorModel.h"

struct HSL : CylindricalColorModel {
    HSL(uint8_t h, uint8_t s, uint8_t l);

    RGB ToRGB() const override;

    uint8_t H() const;

    uint8_t S() const;

    uint8_t L() const;
};


#endif //COLOR_SPACES_HSL_H
