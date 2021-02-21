//
// Created by borisshapa on 21.02.2021.
//

#include <fstream>
#include <iostream>
#include <cmath>
#include "Image.h"
#include "../pixels/Rgb.h"

namespace pnm {
    Image read(const std::string &file_name) {
        std::ifstream in;
        in.open(file_name, std::ios::binary);

        if (in.fail()) {
            std::cerr << "Failed";
        }

        std::string format;
        size_t width, height, max_channel_value;

        in >> format;
        in >> width >> height;
        in >> max_channel_value;

        Pixel *pixels = new Rgb[width * height];

        for (size_t i = 0; i < width * height; i++) {
            pixels[i].ReadFromStream(in);
        }

        return {width, height, pixels};
    }
}