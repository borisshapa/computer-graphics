//
// Created by borisshapa on 06.04.2021.
//

#ifndef IMAGE_DITHERING_UTILITYCONSTANTS_H
#define IMAGE_DITHERING_UTILITYCONSTANTS_H

static const char *INPUT_FORMAT = "The following input data format is supported: <input file name> "
                                  "<output file name> <gradient> <gradient> <bitness> <gamma>\n";

static const char *GRADIENT_FORMAT = "Gradient value must be 0 or 1.\n"
                                     "  0 - using the input image;\n"
                                     "  1 - using gradient (0-255)\n";

static const char *DITHERING_TYPE_FORMAT = "The following types of dithering are supported: \n"
                                           "    0 - No dithering;\n"
                                           "    1 - Ordered (8x8);\n"
                                           "    2 - Random;\n"
                                           "    3 - Floyd–Steinberg;\n";
#endif //IMAGE_DITHERING_UTILITYCONSTANTS_H
