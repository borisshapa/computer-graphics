//
// Created by borisshapa on 31.05.2021.
//

#ifndef PNG_DECODING_PNG_H
#define PNG_DECODING_PNG_H


#include <string>
#include "Image.h"

namespace png {
    Image read(const std::string& file_name);
};


#endif //PNG_DECODING_PNG_H
