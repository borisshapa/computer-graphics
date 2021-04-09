//
// Created by borisshapa on 08.04.2021.
//

#include "FloydSteinbergDithering.h"
#include <algorithm>

std::vector<std::pair<int8_t, int8_t>> FloydSteinbergDithering::COORDINATES_SHIFT = {{0, 1},
                                                                                     {1, -1},
                                                                                     {1, 0},
                                                                                     {1, 1}};

std::vector<double> FloydSteinbergDithering::ERROR_PARTS = {7 / 16., 3 / 16., 5 / 16., 1 / 16.};

Image FloydSteinbergDithering::GetGreyGradient(size_t height, size_t width, size_t bitness) {
    auto *data = new uint8_t[height * width];

    std::vector<std::vector<double>> additions(height, std::vector<double>(width, 0.0));

    for (size_t row = 0; row < height; row++) {
        for (size_t col = 0; col < width; col++) {
            const double addition = additions[row][col];
            const double grey_value_double = GetGreyValueDouble(col, width) + addition;

            uint8_t lower_bound, upper_bound;
            std::tie(lower_bound, upper_bound) = GetLowerUpperBounds(grey_value_double, bitness);

            const double diff = grey_value_double - lower_bound;

            data[row * width + col] = lower_bound;

            for (size_t i = 0; i < COORDINATES_SHIFT.size(); i++) {
                const std::pair<int8_t, int8_t> coordinates_shift = COORDINATES_SHIFT[i];
                const size_t new_row = row + coordinates_shift.first;
                const size_t new_col = col + coordinates_shift.second;
                if (new_row < 0 || new_col < 0 || new_row >= height || new_col >= width) {
                    continue;
                }

                additions[new_row][new_col] += diff * ERROR_PARTS[i];
            }
        }
    }

    return Image(width, height, 1, 255, data);
}