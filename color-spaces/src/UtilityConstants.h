//
// Created by borisshapa on 09.03.2021.
//

#ifndef COLOR_SPACES_UTILITYCONSTANTS_H
#define COLOR_SPACES_UTILITYCONSTANTS_H

#include <string>

static const char *INPUT_FORMAT = "The following input data format is supported:\n"
                                            "    -f <initial color space>\n"
                                            "    -t <final color space>\n"
                                            "    -i <input files count> <input file name>\n"
                                            "    -o <output files count> <output file name>\n";
static const char *REUSING_FLAGS_ERROR_MESSAGE = "Use each flag one at a time";
static const char *FILES_FORMAT = "Use the following format for files description: \n"
                                            "    <files count> - 1 or 3\n"
                                            "    <file name> - \n"
                                            "        for <files count> = 1, just a name of ppm file;\n"
                                            "        for <files count> = 3, a template of the name of the form <name.ext>, "
                                            "which corresponds to files <name_1.ext>, <name_2.ext> and <name_3.ext> for each "
                                            "channel, respectively; pgm format.";

static const char *INITIAL_COLOR_SPACE_FLAG = "-f";
static const char *FINAL_COLOR_SPACE_FLAG = "-t";
static const char *INPUT_FILES_FLAG = "-i";
static const char *OUTPUT_FILES_FLAG = "-o";

static const char *RGB_MODEL_NAME = "RGB";
static const char *HSL_MODEL_NAME = "HSL";
static const char *HSV_MODEL_NAME = "HSV";
static const char *YCBCR601_MODEL_NAME = "YCbCr.601";
static const char *YCBCR709_MODEL_NAME = "YCbCr.709";
static const char *YCOCG_MODEL_NAME = "YCoCg";
static const char *CMY_MODEL_NAME = "CMY";

#endif //COLOR_SPACES_UTILITYCONSTANTS_H
