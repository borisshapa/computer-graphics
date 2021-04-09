//
// Created by borisshapa on 08.04.2021.
//

#ifndef IMAGE_DITHERING_RANDOMDITHERING_H
#define IMAGE_DITHERING_RANDOMDITHERING_H

#include "../img-formats/Image.h"
#include "Gradient.h"

struct RandomDithering : Gradient {
    Image GetGreyGradient(size_t height, size_t width, size_t bitness) override;
};


#endif //IMAGE_DITHERING_RANDOMDITHERING_H
