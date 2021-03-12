//
// Created by borisshapa on 07.03.2021.
//

#ifndef COLOR_SPACES_COLORMODEL_H
#define COLOR_SPACES_COLORMODEL_H


#include <cstdint>
#include <initializer_list>
#include <array>

struct RGB;

struct ColorModel {
    ColorModel();

    ColorModel(const std::initializer_list<uint8_t> &channels_values);

    ColorModel(const ColorModel &color_model);

    ColorModel(ColorModel &&color_model) noexcept;

    ColorModel &operator=(ColorModel &&color_model) noexcept;

    virtual ~ColorModel();

    virtual RGB ToRGB() const = 0;

    uint8_t GetChannelsCount() const;

    const uint8_t *GetChannelValues() const;

    static std::array<double, 3>
    Transform(std::array<std::array<double, 3>, 3> transformation_matrix, std::array<double, 3> vec);

    static uint8_t GetByteValueByZeroOneRangeValue(double value);

    static uint8_t ClampDouble(double value);

private:
    uint8_t channels_count_ = 0;
    uint8_t *channel_values_;
};


#endif //COLOR_SPACES_COLORMODEL_H
