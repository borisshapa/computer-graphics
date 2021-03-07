//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_HSL_H
#define COLOR_SPACES_HSL_H

#include "ColorModel.h"

struct HSL : ColorModel {
    HSL(uint8_t h, uint8_t s, uint8_t l);

    RGB ToRGB() override;

    uint8_t H() const;

    uint8_t S() const;

    uint8_t L() const;
};


#endif //COLOR_SPACES_HSL_H
