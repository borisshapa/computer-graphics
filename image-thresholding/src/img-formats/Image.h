//
// Created by borisshapa on 20.02.2021.
//

#ifndef SIMPLE_TRANSFORMATIONS_IMAGE_H
#define SIMPLE_TRANSFORMATIONS_IMAGE_H

#include <cstdint>
#include <functional>

struct Image {
    Image() = default;

    Image(size_t width, size_t height, uint8_t channels_count, size_t max_channel_value, uint8_t *data);

    Image(const Image &img);

    Image &operator=(Image &&img) noexcept;

    ~Image();

    size_t GetWidth() const;

    size_t GetHeight() const;

    uint8_t GetChannelsCount() const;

    size_t GetMaxChannelValue() const;

    const uint8_t *GetData() const;

    size_t GetRowSize() const;

    size_t GetBytesCount() const;

    Image FlipVertically() const;

    Image FlipHorizontally() const;

    Image Rotate90Right() const;

    Image Rotate90Left() const;

    Image Invert() const;
private:
    Image Rotate(const std::function<size_t(size_t, size_t)> &new_coordinates) const;

    size_t width_ = 0;
    size_t height_ = 0;
    uint8_t channels_count_ = 1;
    size_t max_channel_value_ = 0;
    uint8_t *data_ = nullptr;
};

#endif //SIMPLE_TRANSFORMATIONS_IMAGE_H
