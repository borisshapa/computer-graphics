//
// Created by borisshapa on 08.04.2021.
//

#ifndef IMAGE_DITHERING_FLOYDSTEINBERGDITHERING_H
#define IMAGE_DITHERING_FLOYDSTEINBERGDITHERING_H


#include <vector>
#include "Gradient.h"
#include "../img-formats/Image.h"

struct FloydSteinbergDithering : Gradient {
    Image GetGreyGradient(size_t height, size_t width, size_t bitness) override;

    static std::vector<std::pair<int8_t, int8_t>> COORDINATES_SHIFT;

    static std::vector<double> ERROR_PARTS;
};


#endif //IMAGE_DITHERING_FLOYDSTEINBERGDITHERING_H
