//
// Created by borisshapa on 16.05.2021.
//

#ifndef CROSS_CORRELATION_AMPLITUDEPHASECORRELATION_H
#define CROSS_CORRELATION_AMPLITUDEPHASECORRELATION_H


#include "Correlation.h"
#include "../img-formats/Image.h"

struct AmplitudePhaseCorrelation : Correlation {
    Image GetCrossCorrelationField(const Image &image1, const Image &image2) override;
};


#endif //CROSS_CORRELATION_AMPLITUDEPHASECORRELATION_H
