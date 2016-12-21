#ifndef AUTOCOVARIANCE_H
#define AUTOCOVARIANCE_H

#include <fftw3.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

inline std::vector<double> autocovariance(std::vector<double> array)
{
	std::vector<double> acf_real {};
	int size = signed(array.size());
#ifdef FFTW_ENABLED
	fftw_complex *in, *out;
	fftw_plan p_fft, p_ifft;

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

	for (int i = 0; i < size; ++i) {
		in[i][0] = array[i];
		in[i][1] = 0;
	}

	p_fft = fftw_plan_dft_1d(size, in, out, FFTW_FORWARD,
				 FFTW_ESTIMATE);
	fftw_execute(p_fft);
	fftw_destroy_plan(p_fft);

	for (int i = 0; i < size; ++i) {
		in[i][0] = out[i][0] * out[i][0];
		in[i][1] = 0;
	}

	p_ifft = fftw_plan_dft_1d(size, in, out, FFTW_BACKWARD,
				  FFTW_ESTIMATE);
	fftw_execute(p_ifft);
	fftw_destroy_plan(p_ifft);

	for (int i = 0; i < size; ++i) {
		acf_real.push_back(out[i][0]);
	}

	fftw_free(in);
	fftw_free(out);
#else
	double acf;
	for (int tau = 0; tau < size; ++tau) {
		acf = 0;
		for (int i = 0; i < size - tau; ++i) {
			acf += array[i] * array[i + tau];
		}
		acf_real.push_back(acf);
	}
#endif
	auto mean_lambda = [&](double sum, double elem) {
		return sum + elem / acf_real.size();
	};
	auto mean = std::accumulate(acf_real.begin(), acf_real.end(), 0.0, mean_lambda);

	auto variance_lambda = [&](double sum, double elem) {
		return sum + (elem - mean) * (elem - mean) / acf_real.size();
	};
	auto variance = std::accumulate(acf_real.begin(), acf_real.end(), 0.0, variance_lambda);

	std::transform(acf_real.begin(), acf_real.end(), acf_real.begin(), [&](double elem) {
		return (elem - mean)/variance;
	});

	return acf_real;
}

#endif /* AUTOCOVARIANCE_H */
