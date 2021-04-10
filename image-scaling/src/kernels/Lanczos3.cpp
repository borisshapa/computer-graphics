//
// Created by borisshapa on 10.04.2021.
//

#include <complex>
#include "Lanczos3.h"

Lanczos3::Lanczos3() : Kernel(-3.0, 3.0) {}

double Lanczos3::evaluate(const double x) const {
    if (x == 0.0) {
        return 1.0;
    }

    const double a = 3.0;
    return (x >= GetLowerBound() && x < GetUpperBound())
        ? (a * std::sin(M_PI * x) * std::sin(M_PI * x / a)) / (std::pow(M_PI * x, 2))
        : 0;
}
