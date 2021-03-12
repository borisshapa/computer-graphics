//
// Created by borisshapa on 21.02.2021.
//

#include <fstream>
#include <iostream>
#include "PNMImage.h"

void CheckStream(const std::ios &stream, const std::string &message) {
    if (stream.fail()) {
        throw std::ios_base::failure(message);
    }
}

void CheckStreamAfterOpening(const std::ios &stream) {
    CheckStream(stream, "Error opening file");
}

void CheckStreamAfterClosing(const std::ios &stream) {
    CheckStream(stream, "Error closing file");
}

namespace pnm {
    Image read(const std::string &file_name) {
        std::ifstream in;
        in.open(file_name, std::ios::binary);
        CheckStreamAfterOpening(in);

        std::string format;
        size_t width, height, max_channel_value;
        uint8_t channels_count;

        in >> format;
        if (format == "P5") {
            channels_count = 1;
        } else if (format == "P6") {
            channels_count = 3;
        } else {
            throw std::runtime_error("Unsupported file format: expected P5 or P6 in file header, found: " + format);
        }

        in >> width >> height >> max_channel_value;

        in.ignore(256, '\n');
        const size_t bytes_count = width * height * channels_count;
        char *data = new char[bytes_count];
        in.read(data, bytes_count);

        in.close();
        CheckStreamAfterClosing(in);

        return Image(width, height, channels_count, max_channel_value, reinterpret_cast<uint8_t *>(data));
    }

    void write(const Image &image, const std::string &file_name) {
        std::ofstream out;
        out.open(file_name, std::ios::binary);
        CheckStreamAfterOpening(out);

        const uint8_t channels_count = image.GetChannelsCount();
        if (channels_count == 1) {
            out << "P5";
        } else if (channels_count == 3) {
            out << "P6";
        } else {
            throw std::runtime_error(
                    "Unsupported image format: expected 1 or 3 channels, found: " + std::to_string(channels_count));
        }
        out << '\n';
        size_t width = image.GetWidth();
        size_t height = image.GetHeight();
        out << width << ' ' << height << '\n';
        out << image.GetMaxChannelValue() << '\n';

        out.write(reinterpret_cast<const char *>(image.GetData()), width * height * channels_count);
        out.close();
        CheckStreamAfterClosing(out);
    }
}