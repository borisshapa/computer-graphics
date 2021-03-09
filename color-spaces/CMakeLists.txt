cmake_minimum_required(VERSION 3.17)
project(color_spaces)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/color-models)

add_executable(color_spaces
        src/color-models/CMY.cpp
        src/color-models/CMY.h
        src/color-models/ColorModel.cpp
        src/color-models/ColorModel.h
        src/color-models/CylindricalColorModel.cpp
        src/color-models/CylindricalColorModel.h
        src/color-models/HSL.cpp
        src/color-models/HSL.h
        src/color-models/HSV.cpp
        src/color-models/HSV.h
        src/color-models/RGB.cpp
        src/color-models/RGB.h
        src/color-models/RGBColorSpaceTransformation.cpp
        src/color-models/RGBColorSpaceTransformation.h
        src/color-models/YCbCr601.cpp
        src/color-models/YCbCr601.h
        src/color-models/YCbCr709.cpp
        src/color-models/YCbCr709.h
        src/color-models/YCoCg.cpp
        src/color-models/YCoCg.h

        src/img-formats/Image.cpp
        src/img-formats/Image.h
        src/img-formats/PNMImage.cpp
        src/img-formats/PNMImage.h

        src/main.cpp
        src/UtilityConstants.h)
