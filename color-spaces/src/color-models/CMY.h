//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_CMY_H
#define COLOR_SPACES_CMY_H


#include <cstdint>
#include "ColorModel.h"

struct CMY : ColorModel {
    CMY(uint8_t c, uint8_t m, uint8_t y);

    RGB ToRGB() const override;

    uint8_t C() const;

    uint8_t M() const;

    uint8_t Y() const;
};


#endif //COLOR_SPACES_CMY_H
