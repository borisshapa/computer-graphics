//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_YCOCG_H
#define COLOR_SPACES_YCOCG_H


#include "RGBColorSpaceTransformation.h"

struct YCoCg : RGBColorSpaceTransformation {
    YCoCg(uint8_t y, uint8_t co, uint8_t cg);

    uint8_t Y() const;

    uint8_t Co() const;

    uint8_t Cg() const;

private:
    std::array<std::array<double, 4>, 3> GetTransformationMatrix() const override;
};


#endif //COLOR_SPACES_YCOCG_H
