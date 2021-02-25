//
// Created by borisshapa on 21.02.2021.
//

#include <cstring>
#include "Image.h"

Image::Image(size_t width, size_t height, uint8_t channels_count, size_t max_channel_value, char *data)
        : width_(width),
          height_(height),
          channels_count_(channels_count),
          max_channel_value_(max_channel_value),
          data_(data) {}


Image::Image(const Image &img)
        : width_(img.width_),
          height_(img.height_),
          channels_count_(img.channels_count_),
          max_channel_value_(img.max_channel_value_) {
    size_t bytes_count = img.GetBytesCount();
    data_ = new char[bytes_count];
    std::memcpy(data_, img.data_, bytes_count);
}

Image &Image::operator=(Image &&img) noexcept {
    if (this != &img) {
        delete[] data_;
        width_ = img.width_;
        height_ = img.height_;
        channels_count_ = img.channels_count_;
        max_channel_value_ = img.max_channel_value_;
        data_ = img.data_;

        img.width_ = img.height_ = img.channels_count_ = img.max_channel_value_ = 0;
        img.data_ = nullptr;
    }
    return *this;
}

Image::~Image() {
    delete[] data_;
}

size_t Image::GetWidth() const {
    return width_;
}

size_t Image::GetHeight() const {
    return height_;
}

uint8_t Image::GetChannelsCount() const {
    return channels_count_;
}

size_t Image::GetMaxChannelValue() const {
    return max_channel_value_;
}

const char *Image::GetData() const {
    return data_;
}

size_t Image::GetRowSize() const {
    return width_ * channels_count_;
}

size_t Image::GetBytesCount() const {
    return GetRowSize() * height_;
}

Image Image::FlipVertically() const {
    const size_t row_size = GetRowSize();

    char *new_data = new char[GetBytesCount()];
    char *new_data_last = new_data + GetBytesCount() - row_size;
    for (size_t i = 0; i < height_; i++) {
        std::memcpy(new_data_last - i * row_size, data_ + i * row_size, row_size);
    }

    return Image(width_, height_, channels_count_, max_channel_value_, new_data);
}

Image Image::FlipHorizontally() const {
    const size_t row_size = GetRowSize();

    char *new_data = new char[GetBytesCount()];
    for (size_t row = 0; row < height_; row++) {
        char *new_data_row_last = new_data + (row + 1) * row_size - channels_count_;
        for (size_t pixel = 0; pixel < width_; pixel++) {
            const size_t shift_bytes = pixel * channels_count_;
            std::memcpy(
                    new_data_row_last - shift_bytes,
                    data_ + row * row_size + shift_bytes,
                    channels_count_
            );
        }
    }

    return Image(width_, height_, channels_count_, max_channel_value_, new_data);
}

Image Image::Rotate90Right() const {
    return Rotate([this](size_t row, size_t col) -> size_t {
        return col * (height_ * channels_count_) + (height_ - row - 1) * channels_count_;
    });
}

Image Image::Rotate90Left() const {
    return Rotate([this](size_t row, size_t col) -> size_t {
        return (width_ - col - 1) * (height_ * channels_count_) + row * channels_count_;
    });
}

Image Image::Rotate(const std::function<size_t(size_t, size_t)> &new_coordinates) const {
    const size_t row_size = GetRowSize();

    char *new_data = new char[GetBytesCount()];
    for (size_t row = 0; row < height_; row++) {
        for (size_t col = 0; col < width_; col++) {
            std::memcpy(
                    new_data + new_coordinates(row, col),
                    data_ + row * row_size + col * channels_count_,
                    channels_count_
            );
        }
    }

    return Image(height_, width_, channels_count_, max_channel_value_, new_data);
}

Image Image::Invert() const {
    const size_t bytes_count = GetBytesCount();
    char *new_data = new char[bytes_count];
    for (size_t i = 0; i < bytes_count; i++) {
        new_data[i] = static_cast<char>(max_channel_value_ - data_[i]);
    }
    return Image(width_, height_, channels_count_, max_channel_value_, new_data);
}
