//
// Created by borisshapa on 09.03.2021.
//

#ifndef COLOR_SPACES_CYLINDRICALCOLORMODEL_H
#define COLOR_SPACES_CYLINDRICALCOLORMODEL_H


#include "ColorModel.h"

struct CylindricalColorModel: ColorModel {
    CylindricalColorModel(uint8_t h, uint8_t s, uint8_t x);

protected:
    static RGB ToRGB(double hue, double rgb_min_value, double rgb_max_value);
};


#endif //COLOR_SPACES_CYLINDRICALCOLORMODEL_H
