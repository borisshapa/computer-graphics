//
// Created by borisshapa on 08.04.2021.
//

#include "Gradient.h"
#include <algorithm>

uint8_t Gradient::KeepHighBits(uint8_t number, uint8_t bitness) {
    const uint8_t bitmask = (1 << 8) - (1 << (8 - bitness));
    return number & bitmask;
}

uint8_t Gradient::DuplicateHighBits(uint8_t number, uint8_t bitness) {
    uint8_t high_bits = KeepHighBits(number, bitness);
    double duplicated_high_bits = high_bits / (1 - (1.0 / (1 << bitness)));
    duplicated_high_bits = std::min(duplicated_high_bits, 255.0);
    return static_cast<uint8_t>(duplicated_high_bits);
}

uint8_t Gradient::GetGreyValue(size_t col, size_t width, uint8_t bitness, double addition) {
    double grey_value_double = GetGreyValueDouble(col, width);
    grey_value_double = std::min(grey_value_double, 255.0);

    uint8_t lower_bound, upper_bound;
    std::tie(lower_bound, upper_bound) = GetLowerUpperBounds(grey_value_double, bitness);
    auto round_up = static_cast<uint8_t>(
            static_cast<double>((grey_value_double - lower_bound) / (upper_bound - lower_bound)) + addition
    );
    return (round_up) ? upper_bound : lower_bound;
}

double Gradient::GetGreyValueDouble(size_t col, size_t width) {
    return (static_cast<double>(col) / static_cast<double>(width)) * 255;
}

std::pair<uint8_t, uint8_t> Gradient::GetLowerUpperBounds(double grey_value, uint8_t bitness) {
    if (grey_value >= 255.0) {
        return {255, 255};
    }
    uint8_t step = (1 << (8 - bitness));
    double upper_bound_double = grey_value + step;
    uint8_t upper_bound = (upper_bound_double >= 255.0)
                          ? 255
                          : DuplicateHighBits(static_cast<uint8_t>(upper_bound_double), bitness);
    uint8_t lower_bound = DuplicateHighBits(upper_bound - step, bitness);
    return {lower_bound, upper_bound};
}