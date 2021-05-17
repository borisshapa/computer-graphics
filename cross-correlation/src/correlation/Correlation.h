//
// Created by borisshapa on 16.05.2021.
//

#ifndef CROSS_CORRELATION_CORRELATION_H
#define CROSS_CORRELATION_CORRELATION_H

#include <vector>
#include "../img-formats/Image.h"

struct Correlation {
    virtual ~Correlation() = default;

    virtual Image GetCrossCorrelationField(const Image &image1, const Image &image2) = 0;

protected:
    static Image normalizeCrossCorrelationData(const std::vector<std::vector<double>>& cross_correlation_field);
};

#endif //CROSS_CORRELATION_CORRELATION_H
