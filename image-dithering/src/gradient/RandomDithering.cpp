//
// Created by borisshapa on 08.04.2021.
//

#include "RandomDithering.h"
#include <random>

Image RandomDithering::GetGreyGradient(size_t height, size_t width, size_t bitness) {
    auto *data = new uint8_t[height * width];

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0, 1);

    for (size_t row = 0; row < height; row++) {
        for (size_t col = 0; col < width; col++) {
            const double random_addition = distribution(generator);
            data[row * width + col] = GetGreyValue(col, width, bitness, random_addition);
        }
    }

    return Image(width, height, 1, 255, data);
}
