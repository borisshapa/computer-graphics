//
// Created by borisshapa on 20.02.2021.
//

#ifndef SIMPLE_TRANSFORMATIONS_RGB_H
#define SIMPLE_TRANSFORMATIONS_RGB_H

#include "Pixel.h"

struct Rgb : Pixel {
    char r{};
    char g{};
    char b{};

    void ReadFromStream(std::istream &is) override;
};

#endif //SIMPLE_TRANSFORMATIONS_RGB_H
