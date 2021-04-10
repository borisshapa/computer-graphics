//
// Created by borisshapa on 10.04.2021.
//

#ifndef IMAGE_SCALING_BCSPLINES_H
#define IMAGE_SCALING_BCSPLINES_H


#include "Kernel.h"

struct BCSplines : Kernel {
    BCSplines();

    BCSplines(double b, double c);

    double evaluate(double x) const override;

private:
    double b_ = 0.0;
    double c_ = 0.5;
};


#endif //IMAGE_SCALING_BCSPLINES_H
