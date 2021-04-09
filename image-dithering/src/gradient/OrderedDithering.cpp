//
// Created by borisshapa on 06.04.2021.
//

#include "OrderedDithering.h"


std::array<std::array<double, 8>, 8> OrderedDithering::BAYER_MATRIX_8x8 =
        {{
                 {0, 0.75, 0.1875, 0.9375, 0.046875, 0.796875, 0.234375, 0.984375},
                 {0.5, 0.25, 0.6875, 0.4375, 0.546875, 0.296875, 0.734375, 0.484375},
                 {0.125, 0.875, 0.0625, 0.8125, 0.171875, 0.921875, 0.109375, 0.859375},
                 {0.625, 0.375, 0.5625, 0.3125, 0.671875, 0.421875, 0.609375, 0.359375},
                 {0.03125, 0.78125, 0.21875, 0.96875, 0.015625, 0.765625, 0.203125, 0.953125},
                 {0.53125, 0.28125, 0.71875, 0.46875, 0.515625, 0.265625, 0.703125, 0.453125},
                 {0.15625, 0.90625, 0.09375, 0.84375, 0.140625, 0.890625, 0.078125, 0.828125},
                 {0.65625, 0.40625, 0.59375, 0.34375, 0.640625, 0.390625, 0.578125, 0.328125}
         }};

Image OrderedDithering::GetGreyGradient(size_t height, size_t width, size_t bitness) {
    auto *data = new uint8_t[height * width];

    for (size_t row = 0; row < height; row++) {
        for (size_t col = 0; col < width; col++) {
            const double bayer_element = BAYER_MATRIX_8x8[row % 8][col % 8];
            data[row * width + col] = GetGreyValue(col, width, bitness, bayer_element);
        }
    }

    return Image(width, height, 1, 255, data);
}