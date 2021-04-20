//
// Created by borisshapa on 10.04.2021.
//

#include <valarray>
#include <cmath>
#include "BCSplines.h"

BCSplines::BCSplines(double b, double c) : Kernel(-2.0, 2.0), b_(b), c_(c) {}

BCSplines::BCSplines() : Kernel(-2.0, 2.0), b_(0.0), c_(0.5) {}

double BCSplines::evaluate(const double x) const {
    double abs_x = std::abs(x);
    double result = 0.0;
    if (abs_x < 1) {
        result = (12 - 9 * b_ - 6 * c_) * std::pow(abs_x, 3) + (-18 + 12 * b_ + 6 * c_) * std::pow(abs_x, 2) +
                 (6 - 2 * b_);
    } else if (x < 2) {
        result = (-b_ - 6 * c_) * std::pow(abs_x, 3) + (6 * b_ + 30 * c_) * std::pow(abs_x, 2) +
                 (-12 * b_ - 48 * c_) * abs_x + (8 * b_ + 24 * c_);
    }
    return result / 6;
}
