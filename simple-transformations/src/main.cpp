#include <iostream>
#include "img-formats/Image.h"
#include "img-formats/PNMImage.h"

int main() {
    Image img = pnm::read("../test-imgs/beatles.pnm");
    return 0;
}
