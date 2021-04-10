//
// Created by borisshapa on 10.04.2021.
//

#include <cmath>
#include "NearestNeighbor.h"

uint8_t NearestNeighbor::GetNewImagePixel(
        const size_t row,
        const uint8_t *src_data,
        const size_t src_width,
        const double src_pos
) const {
    auto col = static_cast<size_t>(std::round(src_pos));
    return src_data[row * src_width + col];
}
