//
// Created by sevag on 4/29/15.
//

#include "helper.h"
#include "math.h"

void hanning_window(double *signal, int size) {
    for (int i = 0; i < size; i++) {
        int j = i; // j = index into Hann window function
        signal[i] = (signal[i] * 0.5 * (1.0 - cos(2.0 * M_PI * j / size)));
    }
}

double *zero_pad(double *original_signal, int size) {
    double *new_signal = new double[2*size];
    for (int i = 0; i < size; i++) {
        new_signal[i] = original_signal[i];
    }
    return new_signal;
}
