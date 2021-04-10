//
// Created by borisshapa on 10.04.2021.
//

#include <stdexcept>
#include "Kernel.h"

double Kernel::GetLowerBound() const {
    return lower_bound_;
}

double Kernel::GetUpperBound() const {
    return upper_bound_;
}

Kernel::Kernel(double lower_bound, double upper_bound) {
    if (lower_bound >= 0 || upper_bound <= 0) {
        throw std::runtime_error("Negative lower bound and positive upper bound are expected.");
    }
    lower_bound_ = lower_bound;
    upper_bound_ = upper_bound;
}

