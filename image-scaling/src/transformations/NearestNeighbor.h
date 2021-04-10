//
// Created by borisshapa on 10.04.2021.
//

#ifndef IMAGE_SCALING_NEARESTNEIGHBOR_H
#define IMAGE_SCALING_NEARESTNEIGHBOR_H


#include "SeparableTransformation.h"

struct NearestNeighbor : SeparableTransformation {
protected:
    uint8_t GetNewImagePixel(
            size_t row,
            const uint8_t *src_data,
            size_t src_width,
            const double src_pos
    ) const override;

};


#endif //IMAGE_SCALING_NEARESTNEIGHBOR_H
