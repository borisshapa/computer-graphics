cmake_minimum_required(VERSION 3.19)
project(cross_correlation)

set(CMAKE_CXX_STANDARD 14)

add_executable(cross_correlation
        src/img-formats/Image.cpp
        src/img-formats/Image.h
        src/img-formats/PNMImage.cpp
        src/img-formats/PNMImage.h

        src/correlation/Correlation.cpp
        src/correlation/Correlation.h
        src/correlation/AmplitudePhaseCorrelation.cpp
        src/correlation/AmplitudePhaseCorrelation.h
        src/correlation/PhaseCorrelation.cpp
        src/correlation/PhaseCorrelation.h

        src/fft/FFT.cpp
        src/fft/FFT.h

        src/main.cpp)