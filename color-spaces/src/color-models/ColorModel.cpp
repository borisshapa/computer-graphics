//
// Created by borisshapa on 07.03.2021.
//

#include <cstddef>
#include <cstring>
#include <algorithm>
#include "ColorModel.h"

ColorModel::ColorModel(const std::initializer_list<uint8_t> &channels_values) : channels_count_(
        channels_values.size()) {
    channel_values_ = new uint8_t[channels_count_];
    size_t ind = 0;
    for (auto &channel_value : channels_values) {
        channel_values_[ind] = channel_value;
        ++ind;
    }
}

ColorModel::~ColorModel() {
    delete[] channel_values_;
}

uint8_t ColorModel::GetChannelsCount() const {
    return channels_count_;
}

const uint8_t *ColorModel::GetChannelValues() const {
    return channel_values_;
}

ColorModel::ColorModel() : channels_count_(0), channel_values_(nullptr) {}

ColorModel::ColorModel(const ColorModel &color_model) : channels_count_(color_model.channels_count_) {
    channel_values_ = new uint8_t[channels_count_];
    std::memcpy(channel_values_, color_model.channel_values_, color_model.channels_count_);
}

ColorModel &ColorModel::operator=(ColorModel &&color_model) noexcept {
    if (this != &color_model) {
        delete[] channel_values_;
        channels_count_ = color_model.channels_count_;
        channel_values_ = color_model.channel_values_;

        color_model.channels_count_ = 0;
        color_model.channel_values_ = nullptr;
    }
    return *this;
}

ColorModel::ColorModel(ColorModel &&color_model) noexcept {
    channel_values_ = color_model.channel_values_;
    channels_count_ = color_model.channels_count_;

    channel_values_ = nullptr;
    channels_count_ = 0;
}

uint8_t ColorModel::GetByteValueByZeroOneRangeValue(double value) {
    return ClampDouble(255 * value);
}

uint8_t ColorModel::ClampDouble(double value) {
    return static_cast<uint8_t>(std::max(0.0, std::min(value, 255.0)));
}

std::array<double, 3>
ColorModel::Transform(std::array<std::array<double, 3>, 3> transformation_matrix, std::array<double, 3> vec) {
    return std::array<double, 3>();
}
