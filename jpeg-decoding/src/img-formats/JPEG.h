//
// Created by borisshapa on 05.06.2021.
//

#ifndef JPEG_DECODING_JPEG_H
#define JPEG_DECODING_JPEG_H


#include <string>
#include "Image.h"

namespace jpeg {
    enum MARKER {
        BYTE_FF = 0xFF,
        SOF0 = 0xC0,
        SOF1 = 0xC1, // Start of Frame 1, Extended Sequential DCT
        SOF2 = 0xC2, // Start of Frame 2, Progressive DCT
        SOF3 = 0xC3, // Start of Frame 3, Lossless (Sequential)
        DHT = 0xC4, // Define Huffman Table
        SOF5 = 0xC5, // Start of Frame 5, Differential Sequential DCT
        SOF6 = 0xC6, // Start of Frame 6, Differential Progressive DCT
        SOF7 = 0xC7, // Start of Frame 7, Differential Loessless (Sequential)
        SOF9 = 0xC9, // Extended Sequential DCT, Arithmetic Coding
        SOF10 = 0xCA, // Progressive DCT, Arithmetic Coding
        SOF11 = 0xCB, // Lossless (Sequential), Arithmetic Coding
        SOF13 = 0xCD, // Differential Sequential DCT, Arithmetic Coding
        SOF14 = 0xCE, // Differential Progressive DCT, Arithmetic Coding
        SOF15 = 0xCF, // Differential Lossless (Sequential), Arithmetic Coding
        SOI = 0xD8, // Start of Image
        EOI = 0xD9, // End of Image
        SOS = 0xDA, // Start of Scan
        DQT = 0xDB, // Define Quantization Table
        APP0 = 0xE0, // Application Segment 0, JPEG-JFIF Image
        APP15 = 0xEF, // Application Segment 15
        COM = 0xFE,
        DRI = 0xDD
    };

    static std::array<size_t, 64> zig_zag_map = {{
                                                  0, 1, 8, 16, 9, 2, 3, 10,
                                                  17, 24, 32, 25, 18, 11, 4, 5,
                                                  12, 19, 26, 33, 40, 48, 41, 34,
                                                  27, 20, 13, 6, 7, 14, 21, 28,
                                                  35, 42, 49, 56, 57, 50, 43, 36,
                                                  29, 22, 15, 23, 30, 37, 44, 51,
                                                  58, 59, 52, 45, 38, 31, 39, 46,
                                                  53, 60, 61, 54, 47, 55, 62, 63
                                          }};

    Image read(const std::string &file_name);
}


#endif //JPEG_DECODING_JPEG_H
