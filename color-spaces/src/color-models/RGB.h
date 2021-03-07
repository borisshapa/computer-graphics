//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_RGB_H
#define COLOR_SPACES_RGB_H


#include "ColorModel.h"

struct RGB : ColorModel {
    RGB(uint8_t r, uint8_t g, uint8_t b);

    RGB ToRGB() override;
};


#endif //COLOR_SPACES_RGB_H
