//
// Created by borisshapa on 10.04.2021.
//

#include "Bilinear.h"

double Bilinear::evaluate(const double x) const {
    const double lower_bound = GetLowerBound();
    const double upper_bound = GetUpperBound();
    if (x <= 0) {
        return (lower_bound - x) / lower_bound;
    } else {
        return (upper_bound - x) / upper_bound;
    }
}

Bilinear::Bilinear() : Kernel(-1.0, 1.0) {}
