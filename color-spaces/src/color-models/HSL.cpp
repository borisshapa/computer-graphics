//
// Created by borisshapa on 07.03.2021.
//

#include <cstdlib>
#include "HSL.h"

RGB HSL::ToRGB() {

}

HSL::HSL(uint8_t h, uint8_t s, uint8_t l) : ColorModel{h, s, l} {}

uint8_t HSL::H() const {
    return GetChannelValues()[0];
}

uint8_t HSL::S() const {
    return GetChannelValues()[1];
}

uint8_t HSL::L() const {
    return GetChannelValues()[2];
}
