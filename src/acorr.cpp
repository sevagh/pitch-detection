#include <complex>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <pitch_detection_priv.h>
#include <cassert>
#include <iostream>
extern "C" {
#include <fftw3.h>
}

std::vector<double>
_acorr_r(const std::vector<double> &signal, bool normalize);

std::vector<double>
acorr_r(const std::vector<double> &signal)
{
	return _acorr_r(signal, false);
}

std::vector<double>
acorr_r(const std::vector<double> &signal, bool normalize)
{
	return _acorr_r(signal, normalize);
}

std::vector<double>
_acorr_r(const std::vector<double> &signal, bool normalize)
{
	int N = signal.size();
	int N2 = 2 * N - 1;

	/* https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2 */
	if (N2 & (N2 - 1)) {
		N2--;
		N2 |= N2 >> 1;
		N2 |= N2 >> 2;
		N2 |= N2 >> 4;
		N2 |= N2 >> 8;
		N2 |= N2 >> 16;
		N2++;
	}

	assert(!(N2 & (N2 - 1)));

	std::vector<double> signala_ext(signal);
	std::vector<double> signalb_ext(signal);

	zero_pad(signala_ext, N2 - N, 0); //signala on the right
	zero_pad(signalb_ext, N2 - N, N - 1); //signalb on the left

	std::vector<std::complex<double>> outa(N2, {0.0, 0.0});
	std::vector<std::complex<double>> outb(N2, {0.0, 0.0});
	std::vector<std::complex<double>> out(N2, {0.0, 0.0});
	std::vector<double> result(N2, 0.0);

	fftw_plan pa =
	    fftw_plan_dft_r2c_1d(N2, reinterpret_cast<double *>(signala_ext.data()),
	        reinterpret_cast<fftw_complex *>(outa.data()), FFTW_ESTIMATE);
	fftw_plan pb =
	    fftw_plan_dft_r2c_1d(N2, reinterpret_cast<double *>(signalb_ext.data()),
	        reinterpret_cast<fftw_complex *>(outb.data()), FFTW_ESTIMATE);
	fftw_plan px =
	    fftw_plan_dft_c2r_1d(N2, reinterpret_cast<fftw_complex *>(out.data()),
	        reinterpret_cast<double *>(result.data()), FFTW_ESTIMATE);

	fftw_execute(pa);
	fftw_execute(pb);

	std::complex<double> scale = {1.0 / (double)N2, 0.0};
	for (int i = 0; i < N2; ++i)
		out[i] = outa[i] * conj(outb[i]) * scale;

	fftw_execute(px);

	fftw_destroy_plan(pa);
	fftw_destroy_plan(pb);
	fftw_destroy_plan(px);

	fftw_cleanup();

	if (normalize) {
		std::vector<double> normalized_result(N, 0.0);
		for (int i = 0; i < N; ++i)
			normalized_result[i] = result[i + (N2 - N)] / result[N2 - N];
		return normalized_result;
	}

	return result;
}
