//
// Created by borisshapa on 10.04.2021.
//

#include <stdexcept>
#include <cmath>
#include <vector>
#include "KernelTransformation.h"
#include "../UtilityConstants.h"

uint8_t TrimToByte(const double value) {
    return static_cast<uint8_t>(std::max(0.0, std::min(255.0, value)));
}

KernelTransformation::KernelTransformation(Kernel *kernel) : kernel_(kernel) {}

uint8_t KernelTransformation::GetNewImagePixel(
        const size_t row,
        const uint8_t *src_data,
        const size_t src_width,
        const double src_pos
    ) const {
    double intpart;
    const double shift = modf(src_pos, &intpart);

    std::vector<std::pair<int, double>> coefficients;
    for (int i = static_cast<int>(kernel_->GetLowerBound() + shift); i <= kernel_->GetUpperBound() + shift; i++) {
        coefficients.emplace_back(i, kernel_->evaluate(i - shift));
    }
    double interpolation = 0.0;
    for (auto coef : coefficients) {
        size_t pos = static_cast<size_t>(
                std::max(0, std::min(static_cast<int>(src_width - 1), static_cast<int>(intpart) + coef.first))
        );
        interpolation += coef.second * src_data[row * src_width + pos];
    }

    return TrimToByte(interpolation);
}
