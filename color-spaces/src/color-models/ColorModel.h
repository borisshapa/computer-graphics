//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_COLORMODEL_H
#define COLOR_SPACES_COLORMODEL_H


#include <cstdint>
#include <initializer_list>
#include "RGB.h"

struct ColorModel {
    ColorModel(std::initializer_list<uint8_t> channels_values);

    ~ColorModel();

    virtual RGB ToRGB() = 0;

    uint8_t GetChannelsCount() const;

    const uint8_t *GetChannelValues() const;

private:
    uint8_t channels_count_ = 0;
    uint8_t *channel_values_;
};


#endif //COLOR_SPACES_COLORMODEL_H
