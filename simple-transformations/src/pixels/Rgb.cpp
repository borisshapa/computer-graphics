//
// Created by borisshapa on 21.02.2021.
//

#include <istream>
#include "Rgb.h"

void Rgb::ReadFromStream(std::istream &is) {
    is.read(reinterpret_cast<char *>(this), sizeof(this));
}
