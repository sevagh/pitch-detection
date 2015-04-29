//
// Created by sevag on 4/28/15.
//

#include "goertzel.h"
#include <math.h>

//frequency limits to loop around
//dumb goertzel
#define loop(x) for(double x = 80; x < 10000; x += 0.01)

goertzel::goertzel(double sampling_rate, int size) {
    goertzel::sampling_rate = sampling_rate;
    goertzel::data_size = size;
}

double goertzel::goertzel_energy(int frequency, double *arr, int N) {
    float floatN = (float) N;
    float floatf = (float) frequency;

    float goertzel_k = (floatN * floatf * (float) (1.0f / sampling_rate));
    float goertzel_w = (2.0 * (float) cos(2.0 * M_PI * goertzel_k / floatN));
    float goertzel_re = ((float) cos(2.0 * M_PI * goertzel_k / floatN));
    float goertzel_im = ((float) sin(2.0 * M_PI * goertzel_k / floatN));

    float Q0 = 0.0;
    float Q1 = 0.0;
    float Q2;

    float re;
    float im;
    float E;

    int index;

    for (index = 0; index < N; index++) {
        Q2 = Q1;
        Q1 = Q0;
        Q0 = (float) arr[index] + goertzel_w * Q1 - Q2;
    }

    re = Q0 - goertzel_re * Q1;
    im = goertzel_im * Q1;
    E = re * re + im * im;

    return E / (floatN * 0.5f);
}

float goertzel::get_snr(int frequency, double *arr, int N) {
    float snr;

    int index;

    double total;
    double E;

    total = 0;
    for (index = 0; index < N; index++)
        total += arr[index] * arr[index];

    E = goertzel_energy(frequency, arr, N);

    snr = 10.0f * log10((float) E / (float) (fabs(total - E)));

    return snr;
}

double goertzel::get_pitch(double *data) {
    double freq_max = 0.0f;
    double snr_max = -1000.0f;

    loop(freq) {
        double snr_current = get_snr(freq, data, data_size);
        if (snr_current > snr_max) {
            snr_max = snr_current;
            freq_max = freq;
        }
    }

    return freq_max;
}
