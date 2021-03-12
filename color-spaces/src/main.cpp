//
// Created by borisshapa on 07.03.2021.
//

#include <cstddef>
#include <iostream>
#include <functional>
#include <cstring>
#include "color-models/HSV.h"
#include "color-models/YCbCr601.h"
#include "color-models/YCbCr709.h"
#include "color-models/YCoCg.h"
#include "color-models/CMY.h"
#include "color-models/HSL.h"
#include "color-models/RGB.h"
#include "img-formats/Image.h"
#include "UtilityConstants.h"
#include "img-formats/PNMImage.h"

long int StrToInt(const std::string &input) {
    char *endptr = nullptr;
    errno = 0;
    const long int transformation = std::strtol(input.c_str(), &endptr, 10);
    if (input.c_str() == endptr
        || errno != 0) {
        throw std::runtime_error("Number expected, found: " + input);
    }
    return transformation;
}

void ConvertPixel(uint8_t *pixel, const std::string &from, const std::string &to) {
    uint8_t b1 = pixel[0];
    uint8_t b2 = pixel[1];
    uint8_t b3 = pixel[2];
    ColorModel *cm_pixel;
    if (from == RGB_MODEL_NAME) {
        cm_pixel = new RGB(b1, b2, b3);
    } else if (from == HSL_MODEL_NAME) {
        cm_pixel = new HSL(b1, b2, b3);
    } else if (from == HSV_MODEL_NAME) {
        cm_pixel = new HSV(b1, b2, b3);
    } else if (from == YCBCR601_MODEL_NAME) {
        cm_pixel = new YCbCr601(b1, b2, b3);
    } else if (from == YCBCR709_MODEL_NAME) {
        cm_pixel = new YCbCr709(b1, b2, b3);
    } else if (from == YCOCG_MODEL_NAME) {
        cm_pixel = new YCoCg(b1, b2, b3);
    } else if (from == CMY_MODEL_NAME) {
        cm_pixel = new CMY(b1, b2, b3);
    } else {
        throw std::runtime_error("Unsupported color model");
    }

    const RGB rgb = cm_pixel->ToRGB();
    // TODO: different types of output color space

    RGB output_color_model = rgb.ToRGB();
    const uint8_t *bytes = output_color_model.GetChannelValues();

    std::copy(bytes, bytes + 3, pixel);
}

std::pair<std::string, std::string> SplitIntoNameAndExtension(const std::string &file_name) {
    const size_t dot_index = file_name.find('.');
    const std::string name = file_name.substr(0, dot_index);
    const std::string extension = file_name.substr(dot_index);

    return {name, extension};
}

std::string GetFileNameByNumber(const std::string &file_name, const std::string &extension, const size_t number) {
    return file_name + "_" + std::to_string(number) + extension;
}

Image GetInputImage(const long int input_files_count, const std::string &input_file_name) {
    Image input_image;
    if (input_files_count == 1) {
        input_image = pnm::read(input_file_name);
        if (input_image.GetChannelsCount() != 3 || input_image.GetBytesCount() % 3 != 0) {
            throw std::runtime_error("Expected 3 channel image, the number of bytes must be a multiple of 3");
        }
    } else {
        std::string file_name, extension;
        std::tie(file_name, extension) = SplitIntoNameAndExtension(input_file_name);

        const Image input_image_1 = pnm::read(GetFileNameByNumber(file_name, extension, 1));
        const Image input_image_2 = pnm::read(GetFileNameByNumber(file_name, extension, 2));
        const Image input_image_3 = pnm::read(GetFileNameByNumber(file_name, extension, 3));

        if (input_image_1.GetChannelsCount() != 1 || input_image_2.GetChannelsCount() != 1 ||
            input_image_3.GetChannelsCount() != 1) {
            throw std::runtime_error("Each of the 3 images must have one channel");
        }
        if (input_image_1.GetBytesCount() != input_image_2.GetBytesCount()
            || input_image_1.GetBytesCount() != input_image_3.GetBytesCount()) {
            throw std::runtime_error("Expected that 3 files have the same number of bytes");
        }
        if (input_image_1.GetWidth() != input_image_2.GetWidth()
            || input_image_1.GetWidth() != input_image_3.GetWidth()) {
            throw std::runtime_error("Expected that 3 files have the same width");
        }
        if (input_image_1.GetHeight() != input_image_2.GetHeight()
            || input_image_1.GetHeight() != input_image_3.GetHeight()) {
            throw std::runtime_error("Expected that 3 files have the same height");
        }

        auto *combined_data = new uint8_t[input_image_1.GetBytesCount() * 3];
        for (size_t i = 0; i < input_image_1.GetBytesCount(); i++) {
            const size_t cd_index = 3 * i;
            combined_data[cd_index] = input_image_1.GetData()[i];
            combined_data[cd_index + 1] = input_image_2.GetData()[i];
            combined_data[cd_index + 2] = input_image_3.GetData()[i];
        }

        input_image = Image(input_image_1.GetWidth(), input_image_1.GetHeight(), 3, 255, combined_data);
    }
    return input_image;
}

void WriteConvertedData(const long int output_files_count, const std::string &output_file_name, const size_t width,
                        const size_t height, uint8_t *data) {
    if (output_files_count == 1) {
        Image output_image = Image(width, height, 3, 255, data);
        pnm::write(output_image, output_file_name);
    } else {
        const size_t bytes_count = width * height;
        uint8_t *output_image_data[3];

        for (auto &channel : output_image_data) {
            channel = new uint8_t[bytes_count];
        }
        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < bytes_count; j++) {
                output_image_data[i][j] = data[3 * j + i];
            }
        }

        std::string file_name, extension;
        std::tie(file_name, extension) = SplitIntoNameAndExtension(output_file_name);
        for (size_t i = 0; i < 3; i++) {
            Image output_image = Image(width, height, 1, 255, output_image_data[i]);
            pnm::write(output_image, GetFileNameByNumber(file_name, extension, i + 1));
        }
    }
}

int main(int argc, char *argv[]) {
    std::string initial_color_space, final_color_space, input_file_name, output_file_name;
    long int input_files_count = -1, output_file_count = -1;

    try {
        auto argc_size_t = static_cast<size_t>(argc);
        for (size_t i = 1; i < argc_size_t; i++) {
            if (strcmp(argv[i], INITIAL_COLOR_SPACE_FLAG) == 0) {
                if (!initial_color_space.empty()) {
                    throw std::runtime_error(REUSING_FLAGS_ERROR_MESSAGE);
                }
                if (i + 1 >= argc_size_t) {
                    throw std::runtime_error(INPUT_FORMAT);
                }
                initial_color_space = argv[i + 1];
                i++;
            } else if (strcmp(argv[i], FINAL_COLOR_SPACE_FLAG) == 0) {
                if (!final_color_space.empty()) {
                    throw std::runtime_error(REUSING_FLAGS_ERROR_MESSAGE);
                }
                if (i + 1 >= argc_size_t) {
                    throw std::runtime_error(INPUT_FORMAT);
                }
                final_color_space = argv[i + 1];
                i++;
            } else if (strcmp(argv[i], INPUT_FILES_FLAG) == 0) {
                if (input_files_count != -1) {
                    throw std::runtime_error(REUSING_FLAGS_ERROR_MESSAGE);
                }
                if (i + 2 >= argc_size_t) {
                    throw std::runtime_error(INPUT_FORMAT);
                }
                input_files_count = StrToInt(argv[i + 1]);
                input_file_name = argv[i + 2];
                i += 2;
            } else if (strcmp(argv[i], OUTPUT_FILES_FLAG) == 0) {
                if (output_file_count != -1) {
                    throw std::runtime_error(REUSING_FLAGS_ERROR_MESSAGE);
                }
                if (i + 2 >= argc_size_t) {
                    throw std::runtime_error(INPUT_FORMAT);
                }
                output_file_count = StrToInt(argv[i + 1]);
                output_file_name = argv[i + 2];
                i += 2;
            } else {
                throw std::runtime_error(INPUT_FORMAT);
            }
        }

        if (initial_color_space.empty()) {
            initial_color_space = RGB_MODEL_NAME;
        }
        if (final_color_space.empty()) {
            final_color_space = RGB_MODEL_NAME;
        }

        if (input_files_count != 1 && input_files_count != 3) {
            throw std::runtime_error(FILES_FORMAT);
        }
        if (output_file_count != 1 && output_file_count != 3) {
            throw std::runtime_error(FILES_FORMAT);
        }

        Image input_image = GetInputImage(input_files_count, input_file_name);

        const size_t bytes_count = input_image.GetBytesCount();
        auto *converted_data = new uint8_t[bytes_count];
        std::copy(input_image.GetData(), input_image.GetData() + bytes_count, converted_data);
        for (size_t i = 0; i < bytes_count; i += 3) {
            ConvertPixel(converted_data + i, initial_color_space, final_color_space); // change converted data
        }

        WriteConvertedData(output_file_count, output_file_name, input_image.GetWidth(), input_image.GetHeight(),
                           converted_data);
    } catch (const std::bad_alloc &e) {
        std::cerr << "Allocation failed: " << e.what() << '\n';
        return 1;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}