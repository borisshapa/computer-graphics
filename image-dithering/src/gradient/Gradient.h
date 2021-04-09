//
// Created by borisshapa on 06.04.2021.
//

#ifndef IMAGE_DITHERING_GRADIENT_H
#define IMAGE_DITHERING_GRADIENT_H

#include "../img-formats/Image.h"

struct Gradient {
    virtual ~Gradient() = default;

    virtual Image GetGreyGradient(size_t height, size_t width, size_t bitness) = 0;

    static uint8_t KeepHighBits(uint8_t number, uint8_t bitness);

    static uint8_t DuplicateHighBits(uint8_t number, uint8_t bitness);

    static uint8_t GetGreyValue(size_t col, size_t width, uint8_t bitness, double addition = 0.0);

    static double GetGreyValueDouble(size_t col, size_t width);

    static std::pair<uint8_t, uint8_t> GetLowerUpperBounds(double grey_value, uint8_t bitness);
};


#endif //IMAGE_DITHERING_GRADIENT_H
