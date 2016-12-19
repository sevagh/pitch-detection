#include <stdio.h>
#include <fftw3.h>
#include <stdlib.h>
#include <vector>
#include "autocorrelation.h"
#include "helper.h"

static double get_acf_periodicity(std::vector<double> data, int size)
{
	int starting_index = 0;
	int total_peak_bin_index = 0;
	int occurences = 0;

	for (int i = 1; i < size-1; i++) {
		if ((data[i] > data[i-1]) && (data[i]) > data[i+1]) {
			occurences += 1;
			total_peak_bin_index +=
				(i - starting_index);
			starting_index = i;
		}
	}

	return (double) (total_peak_bin_index) / (double) occurences;
}

std::vector<double> autocorrelate(std::vector<double> data)
{
	std::vector<double> acf_real;
	fftw_complex *in, *out;
	fftw_plan p_fft, p_ifft;
    ssize_t size = data.size();

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

	for (int i = 0; i < size; i++) {
		in[i][0] = data[i];
		in[i][1] = 0;
	}

	p_fft = fftw_plan_dft_1d(size, in, out, FFTW_FORWARD,
				 FFTW_ESTIMATE);
	fftw_execute(p_fft);
	fftw_destroy_plan(p_fft);

	for (int i = 0; i < size; i++) {
		in[i][0] = out[i][0] * out[i][0];
		in[i][1] = 0;
	}

	p_ifft = fftw_plan_dft_1d(size, in, out, FFTW_BACKWARD,
				  FFTW_ESTIMATE);
	fftw_execute(p_ifft);
	fftw_destroy_plan(p_ifft);

	for (int i = 0; i < size; i++) {
		acf_real.push_back(out[i][0]);
	}

	fftw_free(in);
	fftw_free(out);

	return acf_real;
}

double get_pitch_autocorrelation(std::vector<double> data, int sample_rate)
{
    ssize_t size = data.size();
	zero_pad(data);
	std::vector<double> acf = autocorrelate(data);

	double max = -100.0;
	for (int i = 0; i < size*2; i++) {
		if (acf[i] > max) {
			max = acf[i];
		}
	}

	acf[0] = 0.0;

	for (int i = 0; i < size*2; i++) {
		acf[i] = acf[i]/max;
	}

	double peak_bin_index_periodicity = get_acf_periodicity(acf, size*2);

	return (sample_rate/peak_bin_index_periodicity);
}
