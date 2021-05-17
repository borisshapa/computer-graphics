//
// Created by borisshapa on 16.05.2021.
//
#include <limits>
#include <algorithm>
#include "Correlation.h"

Image Correlation::normalizeCrossCorrelationData(const std::vector<std::vector<double>>& cross_correlation_field) {
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();
    for (const auto &cross_correlation_field_row : cross_correlation_field) {
        for (const auto &cross_correlation : cross_correlation_field_row) {
            min = std::min(min, cross_correlation);
            max = std::max(max, cross_correlation);
        }
    }

    const double range = max - min;

    const size_t cross_correlation_field_height = cross_correlation_field.size();
    const size_t cross_correlation_field_width = cross_correlation_field.back().size();
    auto *cross_correlation_field_image = new uint8_t[cross_correlation_field_height * cross_correlation_field_width];
    for (size_t i = 0; i < cross_correlation_field_height; i++) {
        for (size_t j = 0; j < cross_correlation_field_width; j++) {
            cross_correlation_field_image[i * cross_correlation_field_width + j] = static_cast<uint8_t>(
                    std::max(0.0, std::min(255.0, ((cross_correlation_field[i][j] - min) / range) * 255))
            );
        }
    }

    return Image(cross_correlation_field_width, cross_correlation_field_height, 1, 255, cross_correlation_field_image);
}