//
// Created by borisshapa on 10.04.2021.
//

#ifndef IMAGE_SCALING_BILINEAR_H
#define IMAGE_SCALING_BILINEAR_H

#include "Kernel.h"

struct Bilinear : Kernel {
    Bilinear();

    double evaluate(double x) const override;
};


#endif //IMAGE_SCALING_BILINEAR_H
