cmake_minimum_required(VERSION 3.19)
project(jpeg_decoding)

set(CMAKE_CXX_STANDARD 14)

add_executable(jpeg_decoding
        src/img-formats/Image.cpp
        src/img-formats/Image.h
        src/img-formats/PNM.cpp
        src/img-formats/PNM.h
        src/img-formats/JPEG.cpp
        src/img-formats/JPEG.h

        src/main.cpp)