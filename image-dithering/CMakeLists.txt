cmake_minimum_required(VERSION 3.17)
project(image_dithering)

set(CMAKE_CXX_STANDARD 14)

add_executable(image_dithering
        src/gradient/Gradient.cpp
        src/gradient/Gradient.h
        src/gradient/OrderedDithering.cpp
        src/gradient/OrderedDithering.h
        src/gradient/FloydSteinbergDithering.cpp
        src/gradient/FloydSteinbergDithering.h
        src/gradient/RandomDithering.cpp
        src/gradient/RandomDithering.h
        src/gradient/SimpleGradient.cpp
        src/gradient/SimpleGradient.h

        src/img-formats/Image.cpp
        src/img-formats/Image.h
        src/img-formats/PNMImage.cpp
        src/img-formats/PNMImage.h

        src/UtilityConstants.h
        src/main.cpp
)