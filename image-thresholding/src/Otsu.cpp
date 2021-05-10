//
// Created by borisshapa on 07.05.2021.
//

#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include "Otsu.h"

void Otsu::brute_force(
        size_t lower_bound,
        size_t upper_bound,
        size_t thresholds_left_to_arrange,
        std::vector<int> &indices
) {
    if (thresholds_left_to_arrange == 0) {
        double sum = 0;
        for (size_t i = 0; i < indices.size() - 1; i++) {
            double prob, exp;
            std::tie(prob, exp) = ProbabilityExpectation(indices[i] + 1, indices[i + 1]);
            sum += prob * exp * exp;
        }
        if (sum > max_sum) {
            max_sum = sum;
            thresholds = indices;
        }
        return;
    }

    for (size_t f = lower_bound; f < upper_bound; f++) {
        indices[indices.size() - thresholds_left_to_arrange - 1] = static_cast<int>(f);
        brute_force(f + 1, upper_bound + 1, thresholds_left_to_arrange - 1, indices);
    }
}

Image Otsu::Segment(const size_t class_count) {
    thresholds.resize(class_count + 1, -1);

    std::vector<int> indices(class_count + 1, -1);
    indices.back() = static_cast<int>(histogram.size() - 1);

    size_t thresholds_left_to_arrange = class_count - 1;
    brute_force(1, histogram.size() - (class_count - 1), thresholds_left_to_arrange, indices);

    std::vector<size_t> color_mapping(input_image_.GetMaxChannelValue() + 1);

    for (size_t class_ind = 0; class_ind < class_count; class_ind++) {
        size_t color = static_cast<uint8_t>(
                (static_cast<double>(input_image_.GetMaxChannelValue()) / static_cast<double>(class_count - 1))
                * static_cast<double>(class_ind)
        );
        std::cout << thresholds[class_ind + 1] << ' ';
        std::fill(
                color_mapping.begin() + static_cast<int>(thresholds[class_ind]) + 1,
                color_mapping.begin() + static_cast<int>(thresholds[class_ind + 1]) + 1,
                color
        );
    }

    auto *new_data = new uint8_t[pixel_count];
    for (size_t byte = 0; byte < pixel_count; byte++) {
        new_data[byte] = color_mapping[input_image_.GetData()[byte]];
    }
    return Image(
            input_image_.GetWidth(),
            input_image_.GetHeight(),
            input_image_.GetChannelsCount(),
            input_image_.GetMaxChannelValue(),
            new_data
    );
}

std::pair<double, double> Otsu::ProbabilityExpectation(size_t ind1, size_t ind2) {
    if (ind1 == ind2) {
        const double probability = static_cast<double>(histogram[ind1]) / static_cast<double>(pixel_count);
        return {probability, ind1};
    }

    std::pair<double, double> &prob_exp = probability_expectation_dp_[ind1][ind2];
    if (prob_exp.first != -1) {
        return prob_exp;
    }

    double probability, expectation;
    std::tie(probability, expectation) = ProbabilityExpectation(ind1, ind2 - 1);
    expectation *= probability;

    double probability_ind2 = static_cast<double>(histogram[ind2]) / static_cast<double>(pixel_count);
    probability += probability_ind2;
    expectation += static_cast<double>(ind2) * probability_ind2;

    prob_exp = {probability, expectation / probability};
    return prob_exp;
}

Otsu::Otsu(const Image &input_image) : input_image_(input_image) {
    pixel_count = input_image_.GetBytesCount();
    histogram.resize(input_image_.GetMaxChannelValue() + 1, 1);
    for (size_t byte = 0; byte < pixel_count; byte++) {
        histogram[input_image_.GetData()[byte]]++;
    }

    probability_expectation_dp_.resize(histogram.size());
    for (auto &row : probability_expectation_dp_) {
        row.resize(histogram.size());
        for (auto &prob_exp : row) {
            prob_exp = {-1, -1};
        }
    }
}
