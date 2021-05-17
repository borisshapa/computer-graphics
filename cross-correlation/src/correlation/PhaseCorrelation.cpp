//
// Created by borisshapa on 16.05.2021.
//

#include <complex>
#include <vector>
#include <cassert>
#include "PhaseCorrelation.h"
#include "../fft/FFT.h"

Image PhaseCorrelation::GetCrossCorrelationField(const Image &image1, const Image &image2) {
    std::vector<std::vector<std::complex<double>>> fft_image1 = fft::fft_for_image(image1);
    std::vector<std::vector<std::complex<double>>> fft_image2 = fft::fft_for_image(image2);

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
                    const std::complex<double> fft1_value = fft_image1[row][col];
                    const std::complex<double> fft2_conj_value = conj(fft_image2[row][col]);
                    cross_correlation += (
                            (fft1_value / std::abs(fft1_value))
                            * (fft2_conj_value / std::abs(fft2_conj_value))
                            * std::exp(
                                    std::complex<double>(0, 1)
                                    * (2 * M_PI)
                                    * (
                                            ((static_cast<double>(row) * static_cast<double>(delta_y)) / static_cast<double>(height))
                                            + ((static_cast<double>(col) * static_cast<double>(delta_x)) / static_cast<double>(width))
                                    )
                            )
                    ).real();
                }
            }
            cross_correlation /= static_cast<double>(height * width);
            cross_correlation_field[delta_y + half_height][delta_x + half_width] = cross_correlation;
        }
    }

    return normalizeCrossCorrelationData(cross_correlation_field);
}
