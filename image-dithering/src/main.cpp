#include <string>
#include <stdexcept>
#include <iostream>
#include "UtilityConstants.h"
#include "img-formats/Image.h"
#include "img-formats/PNMImage.h"
#include "gradient/Gradient.h"
#include "gradient/SimpleGradient.h"
#include "gradient/OrderedDithering.h"
#include "gradient/RandomDithering.h"
#include "gradient/FloydSteinbergDithering.h"

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
        if (argc - 1 != 6) {
            throw std::runtime_error(INPUT_FORMAT);
        }

        const std::string input_file_name = argv[1];
        const std::string output_file_name = argv[2];
        const long int gradient = StrToInt(argv[3]);
        if (gradient != 0 && gradient != 1) {
            throw std::runtime_error(GRADIENT_FORMAT);
        }
        const bool use_gradient = static_cast<bool>(gradient);
        if (!use_gradient) {
            throw std::runtime_error("Simplified version. Use of a gradient is expected.");
        }

        const uint8_t dithering_type = static_cast<uint8_t>(StrToInt(argv[4]));

        const uint8_t dithering_result_bitness = StrToInt(argv[5]);
        if (!(dithering_result_bitness >= 1 && dithering_result_bitness <= 8)) {
            throw std::runtime_error("The bitness of the dithering result is expected from 1 to 8.");
        }

        const double gamma_value = StrToDouble(argv[6]);
        if (gamma_value != 1.0) {
            throw std::runtime_error("Simplified version. A gamma conversion value of 1 is expected.");
        }

        Image input_image = pnm::read(input_file_name);
        if (input_image.GetChannelsCount() != 1) {
            throw std::runtime_error("Single channel image expected,");
        }

        Gradient *dithering;
        switch (dithering_type) {
            case 0:
                dithering = new SimpleGradient();
                break;
            case 1:
                dithering = new OrderedDithering();
                break;
            case 2:
                dithering = new RandomDithering();
                break;
            case 3:
                dithering = new FloydSteinbergDithering();
                break;
            default:
                throw std::runtime_error(DITHERING_TYPE_FORMAT);
        }
        Image grey_gradient = dithering->GetGreyGradient(
            input_image.GetHeight(),
            input_image.GetWidth(),
            dithering_result_bitness
        );
        delete dithering;

        pnm::write(grey_gradient, output_file_name);
    } catch (const std::bad_alloc &e) {
        std::cerr << "Allocation failed: " << e.what() << '\n';
        return 1;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
