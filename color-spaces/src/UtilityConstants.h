//
// Created by borisshapa on 09.03.2021.
//

#ifndef COLOR_SPACES_UTILITYCONSTANTS_H
#define COLOR_SPACES_UTILITYCONSTANTS_H

const std::string INPUT_FORMAT = "The following input data format is supported:\n"
                                 "    -f <initial color space>\n"
                                 "    -t <final color space>\n"
                                 "    -i <input files count> <input file name>\n"
                                 "    -o <output files count> <output file name>\n";
const std::string REUSING_FLAGS_ERROR_MESSAGE = "Use each flag one at a time";
const std::string FILES_FORMAT = "Use the following format for files description: \n"
                                 "    <files count> - 1 or 3\n"
                                 "    <file name> - \n"
                                 "        for <files count> = 1, just a name of ppm file;\n"
                                 "        for <files count> = 3, a template of the name of the form <name.ext>, "
                                 "which corresponds to files <name_1.ext>, <name_2.ext> and <name_3.ext> for each "
                                 "channel, respectively; pgm format.";

const std::string INITIAL_COLOR_SPACE_FLAG = "-f";
const std::string FINAL_COLOR_SPACE_FLAG = "-t";
const std::string INPUT_FILES_FLAG = "-i";
const std::string OUTPUT_FILES_FLAG = "-o";

const std::string RGB_MODEL_NAME = "RGB";
const std::string HSL_MODEL_NAME = "HSL";
const std::string HSV_MODEL_NAME = "HSV";
const std::string YCBCR601_MODEL_NAME = "YCbCr.601";
const std::string YCBCR709_MODEL_NAME = "YCbCr.709";
const std::string YCOCG_MODEL_NAME = "YCoCg";
const std::string CMY_MODEL_NAME = "CMY";

#endif //COLOR_SPACES_UTILITYCONSTANTS_H
