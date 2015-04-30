//
// Created by sevag on 4/28/15.
//

#include <stdio.h>
#include "autocorrelation.h"

autocorrelation::autocorrelation(double sampling_rate, int size) {
    autocorrelation::sampling_rate = sampling_rate;
    autocorrelation::data_size = size;
}

double autocorrelation::get_pitch(double *data) {
    double *nsdf;

    for (int i = 0; i < data_size; i++) {
//        printf("%f\n", nsdf[i]);
    }

    return 0;
}