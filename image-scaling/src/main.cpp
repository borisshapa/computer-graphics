#include <iostream>
#include "UtilityConstants.h"
#include "img-formats/Image.h"
#include "img-formats/PNMImage.h"
#include "kernels/Kernel.h"
#include "kernels/Bilinear.h"
#include "transformations/SeparableTransformation.h"
#include "transformations/NearestNeighbor.h"
#include "transformations/KernelTransformation.h"
#include "kernels/Lanczos3.h"
#include "kernels/BCSplines.h"
#include <cstdlib>

void CheckConversionCorrectness(const std::string &input, const char *endptr) {
    if (input.c_str() == endptr
        || errno != 0) {
        throw std::runtime_error("Number expected, found: " + input);
    }
}

long int StrToInt(const std::string &input) {
    char *endptr = nullptr;
    errno = 0;
    const long int transformation = std::strtol(input.c_str(), &endptr, 10);
    CheckConversionCorrectness(input, endptr);
    return transformation;
}

double StrToDouble(const std::string &input) {
    char *endptr = nullptr;
    errno = 0;
    const double transformation = std::strtod(input.c_str(), &endptr);
    CheckConversionCorrectness(input, endptr);
    return transformation;
}

int main(int argc, char *argv[]) {
    try {
        if (argc - 1 < 8) {
            throw std::runtime_error(INPUT_FORMAT);
        }
        const std::string input_file_name = argv[1];
        const std::string output_file_name = argv[2];
        const long int resulting_width = StrToInt(argv[3]);
        const long int resulting_height = StrToInt(argv[4]);
        if (resulting_height <= 0 || resulting_width <= 0) {
            throw std::runtime_error("The resulting image dimensions are expected to be positive numbers.");
        }

        // Simple version. dx, dy are ignored.
        const double dx = StrToDouble(argv[5]);
        const double dy = StrToDouble(argv[6]);

        const double gamma = StrToDouble(argv[7]);
        if (gamma != 1.0) {
            throw std::runtime_error("Simple version. A gamma value of 1.0 is expected.");
        }
        const long int scaling_method = StrToInt(argv[8]);

        if ((scaling_method != 3 && argc - 1 != 8) || (scaling_method == 3 && argc - 1 > 10)) {
            throw std::runtime_error(INPUT_FORMAT);
        }

        Image image = pnm::read(input_file_name);
        if (image.GetChannelsCount() != 1) {
            throw std::runtime_error("Simple version. Only grayscale images are supported.");
        }

        SeparableTransformation *transformation = nullptr;
        Kernel *kernel = nullptr;
        switch (scaling_method) {
            case 0:
                transformation = new NearestNeighbor();
                break;
            case 1:
                kernel = new Bilinear();
                break;
            case 2:
                kernel = new Lanczos3();
                break;
            case 3: {
                double b = 0.0;
                double c = 0.5;
                if (argc - 1 == 9) {
                    b = StrToDouble(argv[9]);
                }
                if (argc - 1 == 10) {
                    c = StrToDouble(argv[10]);
                }
                kernel = new BCSplines(b, c);
                break;
            }
            default:
                throw std::runtime_error(SCALING_METHOD_FORMAT);
        }
        if (transformation == nullptr) {
            transformation = new KernelTransformation(kernel);
        }
        Image enlarged_image = transformation->Enlarge(image, resulting_width, resulting_height);
        delete kernel;
        delete transformation;

        pnm::write(enlarged_image, output_file_name);
    } catch (const std::bad_alloc &e) {
        std::cerr << "Allocation failed: " << e.what() << '\n';
        return 1;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
