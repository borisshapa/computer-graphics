//
// Created by borisshapa on 06.04.2021.
//

#ifndef IMAGE_DITHERING_ORDEREDDITHERING_H
#define IMAGE_DITHERING_ORDEREDDITHERING_H


#include "Gradient.h"

struct OrderedDithering : Gradient {
    Image GetGreyGradient(size_t height, size_t width, size_t bitness) override;

    static std::array<std::array<double, 8>, 8> BAYER_MATRIX_8x8;
};


#endif //IMAGE_DITHERING_ORDEREDDITHERING_H
