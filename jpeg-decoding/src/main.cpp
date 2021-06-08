#include <iostream>
#include "img-formats/Image.h"
#include "img-formats/PNM.h"
#include "img-formats/JPEG.h"

int main(int argc, char *argv[]) {
    try {
        if (argc - 1 != 2) {
            throw std::runtime_error("Expected input format: <input JPEG file name> <output JPEG file name>");
        }

        const char *const input_file_name = argv[1];
        const char *const output_file_name = argv[2];

        Image input_image = jpeg::read(input_file_name);
        pnm::write(input_image, output_file_name);
    } catch (const std::bad_alloc &e) {
        std::cerr << "Allocation failed: " << e.what() << '\n';
        return 1;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
