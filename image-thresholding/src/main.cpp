//
// Created by borisshapa on 07.05.2021.
//

#include <string>
#include <stdexcept>
#include <iostream>
#include "img-formats/Image.h"
#include "img-formats/PNMImage.h"
#include "Otsu.h"

long int StrToInt(const std::string &input) {
    char *endptr = nullptr;
    errno = 0;
    const long int transformation = std::strtol(input.c_str(), &endptr, 10);
    if (input.c_str() == endptr
        || errno != 0) {
        throw std::runtime_error("Number expected, found: " + input + "\n");
    }
    return transformation;
}

int main(int argc, char *argv[]) {
    try {
        if (argc - 1 != 3) {
            throw std::runtime_error("The following input data format is supported: "
                                     "<input_file_name> "
                                     "<output_file_name> "
                                     "<number_of_classes>\n");
        }
        const std::string input_file_name = argv[1];
        const std::string output_file_name = argv[2];
        const long int class_count = StrToInt(argv[3]);

        if (class_count < 1) {
            throw std::runtime_error("A positive number of classes is expected.\n");
        }

        Image input_image = pnm::read(input_file_name);
        if (input_image.GetChannelsCount() != 1) {
            throw std::runtime_error("A one channel image is expected.\n");
        }
        Otsu otsu = Otsu(input_image);
        Image output_image = otsu.Segment(class_count);
        pnm::write(output_image, output_file_name);
    } catch (const std::bad_alloc &e) {
        std::cerr << "Allocation failed: " << e.what() << '\n';
        return 1;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}