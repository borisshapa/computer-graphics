//
// Created by borisshapa on 10.04.2021.
//

#ifndef IMAGE_SCALING_KERNELTRANSFORMATION_H
#define IMAGE_SCALING_KERNELTRANSFORMATION_H


#include "SeparableTransformation.h"
#include "../kernels/Kernel.h"

struct KernelTransformation : SeparableTransformation {
    explicit KernelTransformation(Kernel *kernel);

    uint8_t GetNewImagePixel(
            size_t row,
            const uint8_t *src_data,
            size_t src_width,
            double src_pos
    ) const override;

private:
    Kernel *kernel_ = nullptr;
};


#endif //IMAGE_SCALING_KERNELTRANSFORMATION_H
