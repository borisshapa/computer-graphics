#include <iostream>
#include <climits>
#include "img-formats/Image.h"
#include "img-formats/PNMImage.h"

int main(int argc, char *argv[]) {
    const std::string TRANSFORMATION_ERROR = "Unsupported transformation type.\n"
                                             "Use: \n"
                                             "    0 - inversion;\n"
                                             "    1 - horizontal flip;\n"
                                             "    2 - vertical flip;\n"
                                             "    3 - rotate 90 degrees clockwise;\n"
                                             "    4 - rotate 90 degrees counterclockwise.";
    try {
        if (argc != 4) {
            throw std::runtime_error(
                    "Expected 3 arguments: <input file name> <output file name> <transformation>, found: " +
                    std::to_string(argc) + "\n");
        }

        const std::string input_file_name = argv[1];
        const std::string output_file_name = argv[2];

        char *endptr = nullptr;
        errno = 0;
        const long int transformation = std::strtol(argv[3], &endptr, 10);
        if (argv[3] == endptr
            || errno != 0) {
            throw std::runtime_error(TRANSFORMATION_ERROR);
        }
        const Image img = pnm::read(input_file_name);
        Image res;
        switch (transformation) {
            case 0:
                res = img.Invert();
                break;
            case 1:
                res = img.FlipHorizontally();
                break;
            case 2:
                res = img.FlipVertically();
                break;
            case 3:
                res = img.Rotate90Right();
                break;
            case 4:
                res = img.Rotate90Left();
                break;
            default:
                throw std::runtime_error(TRANSFORMATION_ERROR);
        }
        pnm::write(res, output_file_name);
    } catch (const std::bad_alloc &e) {
        std::cerr << "Allocation failed: " << e.what() << '\n';
        return 1;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
