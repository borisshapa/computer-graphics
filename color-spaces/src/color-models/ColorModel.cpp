//
// Created by borisshapa on 07.03.2021.
//

#include <cstddef>
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
