//
// Created by borisshapa on 10.04.2021.
//

#ifndef IMAGE_SCALING_LANCZOS3_H
#define IMAGE_SCALING_LANCZOS3_H


#include "Kernel.h"

struct Lanczos3 : Kernel {
    Lanczos3();

    double evaluate(double x) const override;
};


#endif //IMAGE_SCALING_LANCZOS3_H
