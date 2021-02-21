//
// Created by borisshapa on 20.02.2021.
//

#ifndef SIMPLE_TRANSFORMATIONS_IMAGE_H
#define SIMPLE_TRANSFORMATIONS_IMAGE_H

#include "../pixels/Pixel.h"

struct Image {
    Image(size_t width, size_t height, Pixel* pixels);

private:
    size_t width_ = 0;
    size_t height_ = 0;
    Pixel *pixels_;
};

#endif //SIMPLE_TRANSFORMATIONS_IMAGE_H
