//
// Created by borisshapa on 21.02.2021.
//

#ifndef SIMPLE_TRANSFORMATIONS_PNMIMAGE_H
#define SIMPLE_TRANSFORMATIONS_PNMIMAGE_H

#include <string>
#include "Image.h"

namespace pnm {
    Image read(const std::string &file_name);

    void write(const Image &image, const std::string &file_name);
}

#endif //SIMPLE_TRANSFORMATIONS_PNMIMAGE_H
