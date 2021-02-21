//
// Created by borisshapa on 21.02.2021.
//

#include <cstdio>
#include "Image.h"

Image::Image(size_t width, size_t height, Pixel *pixels)
        : width_(width), height_(height), pixels_(pixels) {}