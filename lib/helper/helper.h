//
// Created by sevag on 4/29/15.
//

#ifndef MPM_HELPER_H
#define MPM_HELPER_H

double* zero_pad(double *original_signal, int size);

void hanning_window(double *signal, int size);

double looper(
	      double (*fp)(double, double*, int, double,
			   double (*fp)(double, double*, int, double)),
	      double *data, int data_size, double sampling_rate,
	      double (*fp2)(double, double*, int, double));

double get_snr(double frequency, double *arr, int N, double sampling_rate,
	       double (*fp) (double, double*, int, double));

#endif //MPM_HELPER_H
