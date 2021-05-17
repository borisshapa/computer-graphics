#include <iostream>
#include "correlation/Correlation.h"
#include "correlation/AmplitudePhaseCorrelation.h"
#include "correlation/PhaseCorrelation.h"
#include "img-formats/PNMImage.h"

long int StrToInt(const std::string &input) {
    char *endptr = nullptr;
    errno = 0;
    const long int transformation = std::strtol(input.c_str(), &endptr, 10);
    if (input.c_str() == endptr
        || errno != 0) {
        throw std::runtime_error("Number expected, found: " + input);
    }
    return transformation;
}

int main(int argc, char *argv[]) {
    try {
        if (argc - 1 != 4) {
            throw std::runtime_error("The following input data format is supported: "
                                     "<input_file1_name> <input_file2_name> <output_file> <correlation_type>");
        }
        const std::string input_file1_name = argv[1];
        const std::string input_file2_name = argv[2];
        const std::string output_file_name = argv[3];
        const long int correlation_type = StrToInt(argv[4]);

        Correlation *correlation;
        switch (correlation_type) {
            case 0:
                correlation = new AmplitudePhaseCorrelation();
                break;
            case 1:
                correlation = new PhaseCorrelation();
                break;
            default:
                throw std::runtime_error("The following correlation types are supported: \n"
                                         "    0 - amplitude phase correlation\n"
                                         "    1 - phase correlation");
        }

        Image image1 = pnm::read(input_file1_name);
        Image image2 = pnm::read(input_file2_name);

        if (image1.GetChannelsCount() != 1 || image2.GetChannelsCount() != 1) {
            throw std::runtime_error("Single channel images are expected");
        }
        if (image1.GetHeight() != image2.GetHeight() || image1.GetWidth() != image2.GetWidth()) {
            throw std::runtime_error("Input images are assumed to be the same size");
        }

        Image output_image = correlation->GetCrossCorrelationField(image1, image2);
        delete correlation;


        uint8_t max = 0;
        size_t x = -1;
        size_t y = -1;
        for (size_t i = 0; i < output_image.GetHeight(); i++) {
            for (size_t j = 0; j < output_image.GetWidth(); j++) {
                const size_t pixel_value = output_image.GetPixel(i, j);
                if (pixel_value > max) {
                    max = pixel_value;
                    x = j;
                    y = i;
                }
            }
        }
        std::cout << "Maximum coordinates: (" << x << ", " << y << ") (The top left corner is (0, 0))\n";
        pnm::write(output_image, output_file_name);
    } catch (const std::bad_alloc &e) {
        std::cerr << "Allocation failed: " << e.what() << '\n';
        return 1;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
