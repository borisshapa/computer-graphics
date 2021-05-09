cmake_minimum_required(VERSION 3.19)
project(image_thresholding)

set(CMAKE_CXX_STANDARD 14)

add_executable(image_thresholding
        src/img-formats/Image.cpp
        src/img-formats/Image.h
        src/img-formats/PNMImage.cpp
        src/img-formats/PNMImage.h

        src/Otsu.cpp
        src/Otsu.h

        src/main.cpp)