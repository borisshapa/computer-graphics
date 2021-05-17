//
// Created by borisshapa on 16.05.2021.
//

#include "FFT.h"
#include <cmath>

namespace fft {
    void fft(std::vector<std::complex<double>> &input, bool invert) {
        const size_t input_size = input.size();
        if (input_size == 1) {
            return;
        }

        std::vector<std::complex<double>> odd(input_size / 2);
        std::vector<std::complex<double>> even(input_size / 2);
        for (size_t i = 0, j = 0; i < input_size; i += 2, j++) {
            even[j] = input[i];
            odd[j] = input[i + 1];
        }

        fft(odd, invert);
        fft(even, invert);

        const double ang = 2 * M_PI / static_cast<double>(input_size) * (invert ? -1 : 1);
        std::complex<double> w(1);
        const std::complex<double> wn(cos(ang), sin(ang));
        for (size_t i = 0; i < input_size / 2; i++) {
            input[i] = even[i] + w * odd[i];
            input[i + input_size / 2] = even[i] - w * odd[i];
            if (invert) {
                input[i] /= 2;
                input[i + input_size / 2] /= 2;
            }
            w *= wn;
        }
    }

    void fft(std::vector<std::vector<std::complex<double>>> &input, bool invert) {
        const size_t input_size = input.size();
        for (auto &row : input) {
            fft(row, invert);
            if (invert) {
                normalize(row);
            }
        }

        for (size_t j = 0; j < input.back().size(); j++) {
            std::vector<std::complex<double>> column(input_size);
            for (size_t i = 0; i < input_size; i++) {
                column[i] = input[i][j];
            }
            fft(column, invert);
            if (invert) {
                normalize(column);
            }
            for (size_t i = 0; i < input_size; i++) {
                input[i][j] = column[i];
            }
        }
    }

    void normalize(std::vector<std::complex<double>> &input) {
        for (size_t i = 0; i < input.size(); i++) {
            input[i] /= static_cast<double>(input.size());
        }
    }

    size_t get_nearest_power_of_2(size_t x) {
        size_t result = 1;
        while (result < x) {
            result *= 2;
        }
        return result;
    }

    std::vector<std::vector<std::complex<double>>> fft_for_image(const Image &image) {
        const size_t height = get_nearest_power_of_2(image.GetHeight());
        const size_t width = get_nearest_power_of_2(image.GetWidth());
        std::vector<std::vector<std::complex<double>>> result(height,std::vector<std::complex<double>>(width));
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                if (i < image.GetHeight() && j < image.GetWidth()) {
                    result[i][j] = std::complex<double>(image.GetData()[i * image.GetWidth() + j], 0);
                } else {
                    result[i][j] = 0;
                }
            }
        }
        fft(result, false);

        result.resize(image.GetHeight());
        for (auto &row : result) {
            row.resize(image.GetWidth());
        }
        return result;
    }
}