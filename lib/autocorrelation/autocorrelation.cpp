//
// Created by sevag on 4/28/15.
//

#include <stdio.h>
#include "autocorrelation.h"
#include "../helper/helper.h"
#include <fftw3.h>
#include <stdlib.h>

autocorrelation::autocorrelation(double sampling_rate, int size) {
    autocorrelation::sampling_rate = sampling_rate;
    autocorrelation::data_size = size;
}

double autocorrelation::get_acf_periodicity(double *data) {
}

double *autocorrelation::get_normalized_acf(double *data) {
    double *padded_data = zero_pad(data, data_size);
    double N = 2*data_size;

    double *acf_real;
    fftw_complex *in, *out;
    fftw_plan p_fft, p_ifft;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    acf_real = (double*) malloc(N);

    //signal is real
    for (int i = 0; i < N; i++) {
        in[i][0] = padded_data[0];
        in[i][1] = 0;
    }

    p_fft = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p_fft);
    fftw_destroy_plan(p_fft);

    //out_psd = (abs(out))^2
    for (int i = 0; i < N; i++) {
        in[i][0] = out[i][0] * out[i][0];
        in[i][1] = 0;
    }

    p_ifft = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(p_ifft);
    fftw_destroy_plan(p_ifft);

    for (int i = 0; i < N; i++) {
        acf_real[i] = out[i][0] * out[i][0];
    }

    //fftw_free(in);
    //fftw_free(out);

    return acf_real;
}

double autocorrelation::get_pitch(double *data) {
    double *acf = get_normalized_acf(data);

    printf("Success\n");


    for (int i; i < data_size*2; i++) {
        printf("%f\n", acf[i]);
    }

    return 0;
}