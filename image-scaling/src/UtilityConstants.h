//
// Created by borisshapa on 10.04.2021.
//

#ifndef IMAGE_SCALING_UTILITYCONSTANTS_H
#define IMAGE_SCALING_UTILITYCONSTANTS_H

static const char *INPUT_FORMAT = "The following input data format is supported: <input_file_name> <output_file_name> "
                                  "<resulting_width> <resulting height> <dx> <dy> <gamma> <scaling method> [<B> <C>]";

static const char *SCALING_METHOD_FORMAT = "The following types of scaling are supported: \n"
                                           "    0 - nearest neighbor;\n"
                                           "    1 - bilinear;\n"
                                           "    2 - lanczos3;\n"
                                           "    3 - BC splines.\n";

#endif //IMAGE_SCALING_UTILITYCONSTANTS_H
