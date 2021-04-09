//
// Created by borisshapa on 08.04.2021.
//

#include "SimpleGradient.h"

Image SimpleGradient::GetGreyGradient(size_t height, size_t width, size_t bitness) {
    auto *data = new uint8_t[height * width];

    for (size_t row = 0; row < height; row++) {
        for (size_t col = 0; col < width; col++) {
            data[row * width + col] = GetGreyValue(col, width, bitness);
        }
    }

    return Image(width, height, 1, 255, data);
}
