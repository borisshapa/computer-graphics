//
// Created by borisshapa on 16.05.2021.
//

#include <cassert>
#include <vector>
#include "AmplitudePhaseCorrelation.h"

Image AmplitudePhaseCorrelation::GetCrossCorrelationField(const Image &image1, const Image &image2) {
    assert(image1.GetHeight() == image2.GetHeight());
    assert(image1.GetWidth() == image2.GetWidth());

    const size_t height = image1.GetHeight();
    const size_t width = image1.GetWidth();

    std::vector<std::vector<double>> cross_correlation_field(height,std::vector<double>(width));

    const int half_height = static_cast<int>(height / 2);
    const int half_width = static_cast<int>(width / 2);
    for (int delta_y = -half_height; delta_y < half_height; delta_y++) {
        for (int delta_x = -half_width; delta_x < half_width; delta_x++) {
            double cross_correlation = 0;
            for (size_t row = 0; row < height; row++) {
                for (size_t col = 0; col < width; col++) {
                    cross_correlation +=
                            static_cast<double>(image1.GetPixel(row, col))
                            * static_cast<double>(image2.GetPixel(
                                    (row + delta_y + height) % height,
                                    (col + delta_x + width) % width
                            ));
                }
            }
            cross_correlation /= static_cast<double>(height * width);
            cross_correlation_field[delta_y + half_height][delta_x + half_width] = cross_correlation;
        }
    }

    return normalizeCrossCorrelationData(cross_correlation_field);
}
