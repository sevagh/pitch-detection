#include <algorithm>
#include <complex>
#include <ffts/ffts.h>
#include <numeric>
#include <pitch_detection.h>
#include <pitch_detection_priv.h>
#include <vector>

std::vector<double>
acorr_r(const std::vector<double> &signal)
{
	int N = signal.size();
	int N2 = 2 * N;

	auto fft_forward = ffts_init_1d(N2, FFTS_FORWARD);
	auto fft_backward = ffts_init_1d(N2, FFTS_BACKWARD);

	std::vector<std::complex<float>> outb(N2);
	std::transform(signal.begin(), signal.begin() + N, outb.begin(),
	    [](double x) -> std::complex<double> { return std::complex(x, 0.0); });

	ffts_execute(fft_forward, outb.data(), outb.data());

	std::complex<float> scale = {1.0f / (float)N2, 0.0};
	for (int i = 0; i < N; ++i)
		outb[i] *= std::conj(outb[i]) * scale;

	ffts_execute(fft_backward, outb.data(), outb.data());

	ffts_free(fft_forward);
	ffts_free(fft_backward);

	std::vector<double> real_result(N, 0.0);
	std::transform(outb.begin(), outb.begin() + N, real_result.begin(),
	    [](std::complex<float> cplx) -> double { return std::real(cplx); });

	return real_result;
}

static double
get_acf_periodicity(const std::vector<double> &data)
{
	int size = data.size();
	int total_peak_bin_index = 0;
	int occurences = 0;

	for (int i = 1; i < size - 1; i++) {
		if ((data[i] > data[i - 1]) && (data[i]) > data[i + 1]) {
			occurences += 1;
			total_peak_bin_index = i;
		}
	}

	return (double)(total_peak_bin_index) / (double)occurences;
}

double
pitch::autocorrelation(const std::vector<double> &data, int sample_rate)
{
	double peak_bin_index_periodicity = get_acf_periodicity(acorr_r(data));

	return (sample_rate / peak_bin_index_periodicity);
}
