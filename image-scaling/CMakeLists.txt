cmake_minimum_required(VERSION 3.19)
project(image_scaling)

set(CMAKE_CXX_STANDARD 14)

add_executable(image_scaling
        src/img-formats/Image.cpp
        src/img-formats/Image.h
        src/img-formats/PNMImage.cpp
        src/img-formats/PNMImage.h

        src/kernels/Bilinear.cpp
        src/kernels/Bilinear.h
        src/kernels/Kernel.cpp
        src/kernels/Kernel.h
        src/kernels/Lanczos3.cpp
        src/kernels/Lanczos3.h
        src/kernels/BCSplines.cpp
        src/kernels/BCSplines.h

        src/transformations/KernelTransformation.cpp
        src/transformations/KernelTransformation.h
        src/transformations/SeparableTransformation.cpp
        src/transformations/SeparableTransformation.h
        src/transformations/NearestNeighbor.cpp
        src/transformations/NearestNeighbor.h

        src/UtilityConstants.h
        src/main.cpp
)