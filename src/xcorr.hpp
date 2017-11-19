#ifndef XCORR_H
#define XCORR_H

#include <complex>
#include <cstring>
#include <vector>

extern "C" {
#include <fftw3.h>
}

inline void
xcorr_fftw_c2c(const std::vector<std::complex<double>> &signala,
    const std::vector<std::complex<double>> &signalb,
    std::vector<std::complex<double>> &result, int N)
{
	int N2 = 2 * N - 1;

	std::vector<std::complex<double>> signala_ext(N2);
	std::vector<std::complex<double>> signalb_ext(N2);

	// zero padding
	std::memcpy(signala_ext.data() + (N - 1), signala.data(),
	    sizeof(std::complex<double>) * N);
	std::memcpy(
	    signalb_ext.data(), signalb.data(), sizeof(std::complex<double>) * N);

	std::vector<std::complex<double>> outa(N2);
	std::vector<std::complex<double>> outb(N2);
	std::vector<std::complex<double>> out(N2);

	fftw_plan pa = fftw_plan_dft_1d(N2,
	    reinterpret_cast<fftw_complex *>(signala_ext.data()),
	    reinterpret_cast<fftw_complex *>(outa.data()), FFTW_FORWARD,
	    FFTW_ESTIMATE);
	fftw_plan pb = fftw_plan_dft_1d(N2,
	    reinterpret_cast<fftw_complex *>(signalb_ext.data()),
	    reinterpret_cast<fftw_complex *>(outb.data()), FFTW_FORWARD,
	    FFTW_ESTIMATE);
	fftw_plan px =
	    fftw_plan_dft_1d(N2, reinterpret_cast<fftw_complex *>(out.data()),
	        reinterpret_cast<fftw_complex *>(result.data()), FFTW_BACKWARD,
	        FFTW_ESTIMATE);

	fftw_execute(pa);
	fftw_execute(pb);

	std::complex<double> scale = {1.0 / (2 * N - 1), 0.0};
	std::complex<double> outb_i_tmp;
	for (int i = 0; i < N2; ++i) {
		std::memcpy(&outb_i_tmp, &outb[i], sizeof(fftw_complex));
		out[i] = outa[i] * std::conj(outb_i_tmp) * scale;
	}

	fftw_execute(px);

	fftw_destroy_plan(pa);
	fftw_destroy_plan(pb);
	fftw_destroy_plan(px);

	fftw_cleanup();
	return;
}

inline void
xcorr_fftw_r2c(const std::vector<double> &signala,
    const std::vector<double> &signalb,
    std::vector<std::complex<double>> &result, int N)
{
	int N2 = 2 * N - 1;

	std::vector<double> signala_ext(N2);
	std::vector<double> signalb_ext(N2);

	// zero padding
	std::memcpy(signala_ext.data() + (N - 1), signala.data(), sizeof(double) * N);
	std::memcpy(signalb_ext.data(), signalb.data(), sizeof(double) * N);

	std::vector<std::complex<double>> outa(N2);
	std::vector<std::complex<double>> outb(N2);
	std::vector<std::complex<double>> out(N2);

	fftw_plan pa =
	    fftw_plan_dft_r2c_1d(N2, reinterpret_cast<double *>(signala_ext.data()),
	        reinterpret_cast<fftw_complex *>(outa.data()), FFTW_ESTIMATE);
	fftw_plan pb =
	    fftw_plan_dft_r2c_1d(N2, reinterpret_cast<double *>(signalb_ext.data()),
	        reinterpret_cast<fftw_complex *>(outb.data()), FFTW_ESTIMATE);

	fftw_plan px =
	    fftw_plan_dft_1d(N2, reinterpret_cast<fftw_complex *>(out.data()),
	        reinterpret_cast<fftw_complex *>(result.data()), FFTW_BACKWARD,
	        FFTW_ESTIMATE);

	fftw_execute(pa);
	fftw_execute(pb);

	std::complex<double> scale = {1.0 / (2 * N - 1), 0.0};
	for (int i = 0; i < N2; ++i)
		out[i] = outa[i] * conj(outb[i]) * scale;

	fftw_execute(px);

	fftw_destroy_plan(pa);
	fftw_destroy_plan(pb);
	fftw_destroy_plan(px);

	fftw_cleanup();

	return;
}

inline void
xcorr_fftw_r2r(const std::vector<double> &signala,
    const std::vector<double> &signalb, std::vector<double> &result, int N)
{
	int N2 = 2 * N - 1;
	std::vector<std::complex<double>> result_cmplx(N2);
	xcorr_fftw_r2c(signala, signalb, result_cmplx, N);
	for (int x = 0; x < 2 * N - 1; ++x) {
		double real = std::real(result_cmplx[x]);
		double real_dest = result[x];
		std::memcpy(&real_dest, &real, sizeof(double));
	}
}

inline void
xcorr_timedomain(const void *signala, const void *signalb, void *result, int N)
{
	for (int tau = 0; tau < 2 * N - 1; ++tau) {
		std::complex<double> acf = {0, 0};
		for (int i = 0; i < N; ++i) {
			const int signala_idx = (i + tau) % (2 * N - 1);
			const std::complex<double> conjb =
			    conj(((std::complex<double> *)signalb)[i]);
			const double factor =
			    (signala_idx >= N)
			        ? std::real(
			              ((std::complex<double> *)signala)[signala_idx - N])
			        : 1.0;
			acf += factor * conjb;
		}
		((std::complex<double> *)result)[tau] = acf;
	}
	return;
}

#endif /* XCORR_H */
