//
// Created by borisshapa on 16.05.2021.
//

#ifndef CROSS_CORRELATION_FFT_H
#define CROSS_CORRELATION_FFT_H

#include <vector>
#include <complex>
#include "../img-formats/Image.h"

namespace fft {
    void fft(std::vector<std::complex<double>> &input, bool invert);

    void fft(std::vector<std::vector<std::complex<double>>> &input, bool invert);

    void normalize(std::vector<std::complex<double>> &input);

    std::vector<std::vector<std::complex<double>>> fft_for_image(const Image& image);
}


#endif //CROSS_CORRELATION_FFT_H
