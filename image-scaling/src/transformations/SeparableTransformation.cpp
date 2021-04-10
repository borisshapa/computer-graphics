//
// Created by borisshapa on 10.04.2021.
//

#include <stdexcept>
#include "SeparableTransformation.h"

Image SeparableTransformation::Enlarge(const Image& src, size_t new_width, size_t new_height) const {
    Image enlarged_image = EnlargeHorizontally(src, new_width);
    enlarged_image = enlarged_image.Rotate90Right();
    enlarged_image = EnlargeHorizontally(enlarged_image, new_height);
    return enlarged_image.Rotate90Left();
}

Image SeparableTransformation::EnlargeHorizontally(const Image& src, size_t new_width) const {
    const size_t src_height = src.GetHeight();
    const size_t src_width = src.GetWidth();
    const uint8_t *src_data = src.GetData();

    if (src_width > new_width) {
        throw std::runtime_error("The resulting sizes are expected to be larger than the original sizes.");
    }

    auto *data = new uint8_t[new_width * src_height];
    for (size_t row = 0; row < src_height; row++) {
        for (size_t col = 0; col < new_width; col++) {
            data[row * new_width + col] = GetNewImagePixel(
                    row,
                    src_data,
                    src_width,
                    (static_cast<double>(col) / static_cast<double>(new_width)) * static_cast<double>(src_width)
            );
        }
    }

    return Image(new_width, src_height, 1, 255, data);
}
