//
// Created by borisshapa on 10.04.2021.
//

#ifndef IMAGE_SCALING_SEPARABLETRANSFORMATION_H
#define IMAGE_SCALING_SEPARABLETRANSFORMATION_H


#include "../img-formats/Image.h"

struct SeparableTransformation {
    virtual ~SeparableTransformation() = default;

    Image Enlarge(const Image &src, size_t new_width, size_t new_height) const;

protected:
    virtual uint8_t GetNewImagePixel(
            size_t row,
            const uint8_t *src_data,
            size_t src_width,
            double src_pos
    ) const = 0;

private:
    Image EnlargeHorizontally(const Image &src, size_t new_width) const;
};


#endif //IMAGE_SCALING_SEPARABLETRANSFORMATION_H
