//
// Created by borisshapa on 21.02.2021.
//

#ifndef SIMPLE_TRANSFORMATIONS_PIXEL_H
#define SIMPLE_TRANSFORMATIONS_PIXEL_H

#include <istream>

struct Pixel {
    virtual void ReadFromStream(std::istream &is) = 0;
};

#endif //SIMPLE_TRANSFORMATIONS_PIXEL_H
