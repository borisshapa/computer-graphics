//
// Created by borisshapa on 07.05.2021.
//

#ifndef IMAGE_THRESHOLDING_OTSU_H
#define IMAGE_THRESHOLDING_OTSU_H


#include "img-formats/Image.h"
#include <vector>

struct Otsu {
    explicit Otsu(const Image &input_image);

    Image Segment(size_t class_count);

private:
    Image input_image_;
    std::vector<std::vector<std::pair<double, double>>> probability_expectation_dp_;
    std::vector<size_t> histogram;
    size_t pixel_count;
    double max_sum = 0;
    std::vector<int> thresholds;

    std::pair<double, double> ProbabilityExpectation(size_t ind1, size_t ind2);

    void brute_force(
            size_t lower_bound,
            size_t upper_bound,
            size_t thresholds_left_to_arrange,
            std::vector<int> &indices
    );
};


#endif //IMAGE_THRESHOLDING_OTSU_H
