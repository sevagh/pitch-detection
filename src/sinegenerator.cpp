//
// Created by sevag on 4/27/15.
//

#include <stdlib.h>
#include "sinegenerator.h"
#include "math.h"

#define SIZE 4096

sinegenerator::sinegenerator(double sampling_rate, double frequency) {
    sinegenerator::sampling_rate = sampling_rate;
    sinegenerator::inv_sampling_rate = 1.0 / sinegenerator::sampling_rate;

    sinegenerator::frequency = frequency;

    sinegenerator::tone_dual_channel = new double[SIZE];
    sinegenerator::size_dual_channel = SIZE;

    sinegenerator::tone_single_channel = new double[SIZE / 2];
    sinegenerator::size_single_channel = SIZE / 2;
}

void sinegenerator::generate_tone() {
    int SINE_SIZE = SIZE / 2;
    int LUT_SIZE = SIZE / 4;

    float floatf = (float) frequency;
    float delta_phi = floatf * inv_sampling_rate * LUT_SIZE;
    float phase = 0.0f;

    int phase_i;
    int i, j;

    int sine[SINE_SIZE];
    int LUT[LUT_SIZE];

    for (i = 0; i < LUT_SIZE; i++) {
        LUT[i] = (int) roundf(0x7FFF * sinf(2.0f * M_PI * i / LUT_SIZE));
    }

    for (j = 0; j < SINE_SIZE; j++) {
        phase_i = (int) phase;
        sine[j] = LUT[phase_i];
        phase += delta_phi;
        if (phase >= LUT_SIZE)
            phase -= LUT_SIZE;
    }

    for (j = 0; j < SIZE; j += 2) {
        tone_dual_channel[j] = sine[j / 2];
        tone_dual_channel[j + 1] = tone_dual_channel[j];
        tone_single_channel[j / 2] = sine[j / 2];
    }
}

void sinegenerator::cleanup() {
    delete[] tone_single_channel;
    delete[] tone_dual_channel;
}