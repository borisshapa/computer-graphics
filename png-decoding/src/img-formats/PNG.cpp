//
// Created by borisshapa on 31.05.2021.
//

#include "PNG.h"
#include "zlib.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>

namespace png {
    struct ChunkHeader {
        uint32_t size;
        char type[4];
    } __attribute__((__packed__));

    uint32_t ntohl(uint32_t x) {
        auto *bytes = reinterpret_cast<uint8_t *>(&x);
        uint32_t result = 0;
        for (int i = 0; i < 4; i++) {
            result |= (bytes[i] << ((3 - i) * 8));
        }
        return result;
    }

    struct IHDRChunk {
        uint32_t width;
        uint32_t height;
        uint8_t bit_depth;
        uint8_t color_type;
        uint8_t compression_method;
        uint8_t filter_method;
        uint8_t interlace_method;
    } __attribute__((__packed__));

    const static uint32_t CHUNK_SIZE = 16536;

    ChunkHeader ReadChunkHeader(std::istream &in) {
        ChunkHeader chunk_header{};
        in.read(reinterpret_cast<char *>(&chunk_header), sizeof(ChunkHeader));
        chunk_header.size = ntohl(chunk_header.size);
        return chunk_header;
    }

    int PaethPredictor(int a, int b, int c) {
        int p = a + b - c;
        int pa = std::abs(p - a);
        int pb = std::abs(p - b);
        int pc = std::abs(p - c);
        if (pa <= pb && pa <= pc) {
            return a;
        }
        if (pb <= pc) {
            return b;
        }
        return c;
    }

    Image read(const std::string &file_name) {
        std::ifstream in;
        in.open(file_name, std::ios::binary);

        uint8_t signature[8];
        uint8_t expected_signature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
        in.read(reinterpret_cast<char *>(signature), 8);
        if (!std::equal(signature, signature + 8, expected_signature, expected_signature + 8)) {
            throw std::runtime_error("Invalid format of the png file header");
        }

        uint32_t width, height, channel_count;

        ChunkHeader chunk_header = ReadChunkHeader(in);
        if (std::strncmp(chunk_header.type, "IHDR", 4) != 0) {
            throw std::runtime_error("IHDR chunk is expected at the beginning of the PNG file");
        }

        IHDRChunk ihdr{};
        in.read(reinterpret_cast<char *>(&ihdr), sizeof(IHDRChunk));
        width = ntohl(ihdr.width);
        height = ntohl(ihdr.height);
        switch (ihdr.color_type) {
            case 0:
                channel_count = 1;
                break;
            case 2:
                channel_count = 3;
                break;
            default:
                throw std::runtime_error("Only single-channel and three-channel images are supported");
        }
        in.seekg(4, std::ios::cur);

        std::vector<uint8_t> buffer(CHUNK_SIZE);
        std::vector<uint8_t> compressed_image_data;

        while (in) {
            chunk_header = ReadChunkHeader(in);
            if (strncmp(chunk_header.type, "IDAT", 4) != 0) {
                in.seekg(chunk_header.size + 4, std::ios::cur);
                continue;
            }

            long int chunk_header_size = chunk_header.size;
            if (chunk_header_size > buffer.size()) {
                buffer.resize(chunk_header_size);
            }
            in.read(reinterpret_cast<char *>(buffer.data()), chunk_header_size);

            compressed_image_data.insert(compressed_image_data.end(), buffer.begin(),
                                         buffer.begin() + chunk_header_size);
            in.seekg(4, std::ios::cur);
        }

        size_t row_size = channel_count * width + 1;
        std::vector<uint8_t> filtered_data(row_size * height);
        z_stream stream;
        stream.zalloc = nullptr;
        stream.zfree = nullptr;
        stream.opaque = nullptr;
        stream.avail_in = compressed_image_data.size();
        stream.next_in = compressed_image_data.data();
        stream.avail_out = filtered_data.size();
        stream.next_out = filtered_data.data();

        auto ret = inflateInit(&stream);
        if (ret != Z_OK) {
            throw std::runtime_error("Inflate initializing error");
        }
        ret = inflate(&stream, Z_NO_FLUSH);
        inflateEnd(&stream);
        if (ret != Z_STREAM_END) {
            throw std::runtime_error("Inflate error");
        }

        size_t image_data_row_size = width * channel_count;
        auto *image_data = new uint8_t[height * image_data_row_size];

        for (size_t row = 0; row < height; row++) {
            uint8_t filter = filtered_data[row * row_size];
            switch (filter) {
                case 1:
                    for (size_t i = 0; i < channel_count; i++) {
                        image_data[row * image_data_row_size + i] = filtered_data[row * row_size + i + 1];
                    }
                    for (size_t i = channel_count; i < image_data_row_size; i++) {
                        image_data[row * image_data_row_size + i] =
                                filtered_data[row * row_size + i + 1]
                                + image_data[row * image_data_row_size + i - channel_count];
                    }
                    break;
                case 2:
                    if (row == 0) {
                        for (size_t i = 0; i < image_data_row_size; i++) {
                            image_data[row * image_data_row_size + i] = filtered_data[row * row_size + i + 1];
                        }
                    } else {
                        for (size_t i = 0; i < image_data_row_size; i++) {
                            image_data[row * image_data_row_size + i] =
                                    filtered_data[row * row_size + i + 1]
                                    + image_data[(row - 1) * image_data_row_size + i];
                        }
                    }
                    break;
                case 3:
                    if (row == 0) {
                        for (size_t i = 0; i < channel_count; i++) {
                            image_data[row * image_data_row_size + i] = filtered_data[row * row_size + i + 1];
                        }
                        for (size_t i = channel_count; i < image_data_row_size; i++) {
                            image_data[row * image_data_row_size + i] =
                                    filtered_data[row * row_size + i + 1]
                                    + (image_data[row * image_data_row_size + i - channel_count] >> 1);
                        }
                    } else {
                        for (size_t i = 0; i < channel_count; i++) {
                            image_data[row * image_data_row_size + i] =
                                    filtered_data[row * row_size + i + 1]
                                    + (image_data[(row - 1) * image_data_row_size + i] >> 1);
                        }
                        for (size_t i = channel_count; i < image_data_row_size; i++) {
                            image_data[row * image_data_row_size + i] =
                                    filtered_data[row * row_size + i + 1]
                                    + (
                                            (image_data[row * image_data_row_size + i - channel_count]
                                             + image_data[(row - 1) * image_data_row_size + i]) >> 1
                                    );
                        }
                    }
                    break;
                case 4:
                    if (row == 0) {
                        for (size_t i = 0; i < channel_count; i++) {
                            image_data[row * image_data_row_size + i] = filtered_data[row * row_size + i + 1];
                        }
                        for (size_t i = channel_count; i < image_data_row_size; i++) {
                            image_data[row * image_data_row_size + i] =
                                    filtered_data[row * row_size + i + 1]
                                    + PaethPredictor(image_data[row * image_data_row_size + i - channel_count], 0, 0);
                        }
                    } else {
                        for (size_t i = 0; i < channel_count; i++) {
                            image_data[row * image_data_row_size + i] =
                                    filtered_data[row * row_size + i + 1]
                                    + PaethPredictor(
                                            0,
                                            image_data[(row - 1) * image_data_row_size + i],
                                            0
                                    );
                        }
                        for (size_t i = channel_count; i < image_data_row_size; i++) {
                            image_data[row * image_data_row_size + i] =
                                    filtered_data[row * row_size + i + 1]
                                    + PaethPredictor(
                                            image_data[row * image_data_row_size + i - channel_count],
                                            image_data[(row - 1) * image_data_row_size + i],
                                            image_data[(row - 1) * image_data_row_size + i - channel_count]
                                    );
                        }
                    }
                    break;
                default:
                    for (size_t i = 0; i < image_data_row_size; i++) {
                        image_data[row * image_data_row_size + i] = filtered_data[row * row_size + i + 1];
                    }
                    break;
            }
        }
        return Image(width, height, channel_count, 255, image_data);
    }
}