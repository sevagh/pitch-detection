//
// Created by sevag on 4/28/15.
//

#include "dft.h"
#include <math.h>

//frequency limits to loop around
//dumb dft
#define loop(x) for(double x = 80; x < 10000; x += 0.01)

dft::dft(double sampling_rate, int size) {
    dft::sampling_rate = sampling_rate;
    dft::data_size = size;
}

double dft::dft_energy(int frequency, double *arr, int N)
{
    float real = 0.0f;
    float im = 0.0f;
    float E = 0.0f;

    float floatN = (float) N;
    float floatf = (float) frequency;

    float DFT_k = (floatf*floatN*(float) (1.0/sampling_rate));

    int index;
    for (index = 0; index < N; index++) {
        float ind = (float) index;
        float real_c = cos(2.0f*M_PI*ind*DFT_k/floatN);
        float im_c = sin(2.0f*M_PI*ind*DFT_k/floatN);
        real += (float) arr[index]*real_c;
        im -= (float) arr[index]*im_c;
    }

    E += real*real + im*im;

    return E/(floatN*0.5);
}

float dft::get_snr(int frequency, double *arr, int N) {
    float snr;

    int index;

    double total;
    double E;

    total = 0;
    for (index = 0; index < N; index++)
        total += arr[index] * arr[index];

    E = dft_energy(frequency, arr, N);

    snr = 10.0f * log10((float) E / (float) (fabs(total - E)));

    return snr;
}

double dft::get_pitch(double *data) {
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
