cmake_minimum_required(VERSION 3.17)
project(simple_transformations)

set(CMAKE_CXX_STANDARD 14)

add_executable(simple_transformations src/main.cpp src/pixels/Rgb.h src/img-formats/Image.h src/img-formats/Image.cpp src/img-formats/PNMImage.cpp src/pixels/Pixel.h src/pixels/Rgb.cpp src/img-formats/PNMImage.h)