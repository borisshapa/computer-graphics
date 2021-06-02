cmake_minimum_required(VERSION 3.19)
project(png_decoding)

set(CMAKE_CXX_STANDARD 14)

find_package(ZLIB)

add_executable(png_decoding
        src/img-formats/Image.cpp
        src/img-formats/Image.h
        src/img-formats/PNM.cpp
        src/img-formats/PNM.h
        src/img-formats/PNG.cpp
        src/img-formats/PNG.h

        src/main.cpp)

target_link_libraries(png_decoding ZLIB::ZLIB)