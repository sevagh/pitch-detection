#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstring>
#include <float.h>
#include <numeric>
#include <pitch_detection.h>
#include <pitch_detection_priv.h>
#include <vector>
extern "C" {
#include <fftw3.h>
}

static std::vector<double>
acorr_r(const std::vector<double> &signal)
{
	int N = signal.size();
	int N2 = 2 * N - 1;

	/* https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
	 */
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

	zero_pad(signala_ext, N2 - N, 0); // signala on the right
	zero_pad(signalb_ext, N2 - N, N); // signalb on the left

	auto *outa = fftw_alloc_complex(N2);
	auto *outb = fftw_alloc_complex(N2);
	auto *out = fftw_alloc_complex(N2);
	std::vector<double> result(N2, 0.0);

	fftw_plan pa = fftw_plan_dft_r2c_1d(N2,
	    reinterpret_cast<double *>(signala_ext.data()), outa, FFTW_ESTIMATE);
	fftw_plan pb = fftw_plan_dft_r2c_1d(N2,
	    reinterpret_cast<double *>(signalb_ext.data()), outb, FFTW_ESTIMATE);
	fftw_plan px = fftw_plan_dft_c2r_1d(
	    N2, out, reinterpret_cast<double *>(result.data()), FFTW_ESTIMATE);

	fftw_execute(pa);
	fftw_execute(pb);

	std::complex<double> scale = {1.0 / (double)N2, 0.0};
	for (int i = 0; i < N2; ++i) {
		std::complex<double> a, b;
		memcpy(&a, &outa[i], sizeof(fftw_complex));
		memcpy(&b, &outb[i], sizeof(fftw_complex));
		auto result = a * conj(b) * scale;
		memcpy(&out[i], &result, sizeof(fftw_complex));
	}

	fftw_execute(px);

	fftw_destroy_plan(pa);
	fftw_destroy_plan(pb);
	fftw_destroy_plan(px);

	fftw_cleanup();

	std::vector<double> normalized_result(N, 0.0);
	for (int i = 0; i < N; ++i)
		normalized_result[i] = result[i + (N2 - N)] / result[N2 - N];
	return normalized_result;
}

static std::vector<int>
peak_picking(const std::vector<double> &nsdf)
{
	std::vector<int> max_positions{};
	int pos = 0;
	int cur_max_pos = 0;
	ssize_t size = nsdf.size();

	while (pos < (size - 1) / 3 && nsdf[pos] > 0)
		pos++;
	while (pos < size - 1 && nsdf[pos] <= 0.0)
		pos++;

	if (pos == 0)
		pos = 1;

	while (pos < size - 1) {
		if (nsdf[pos] > nsdf[pos - 1] && nsdf[pos] >= nsdf[pos + 1]) {
			if (cur_max_pos == 0) {
				cur_max_pos = pos;
			} else if (nsdf[pos] > nsdf[cur_max_pos]) {
				cur_max_pos = pos;
			}
		}
		pos++;
		if (pos < size - 1 && nsdf[pos] <= 0) {
			if (cur_max_pos > 0) {
				max_positions.push_back(cur_max_pos);
				cur_max_pos = 0;
			}
			while (pos < size - 1 && nsdf[pos] <= 0.0) {
				pos++;
			}
		}
	}
	if (cur_max_pos > 0) {
		max_positions.push_back(cur_max_pos);
	}
	return max_positions;
}

double
get_pitch_mpm(const std::vector<double> &data, int sample_rate)
{
	std::vector<double> nsdf = acorr_r(data);
	std::vector<int> max_positions = peak_picking(nsdf);
	std::vector<std::pair<double, double>> estimates;

	double highest_amplitude = -DBL_MAX;

	for (int i : max_positions) {
		highest_amplitude = std::max(highest_amplitude, nsdf[i]);
		if (nsdf[i] > MPM_SMALL_CUTOFF) {
			auto x = parabolic_interpolation(nsdf, i);
			estimates.push_back(x);
			highest_amplitude = std::max(highest_amplitude, std::get<1>(x));
		}
	}

	if (estimates.empty())
		return -1;

	double actual_cutoff = MPM_CUTOFF * highest_amplitude;
	double period = 0;

	for (auto i : estimates) {
		if (std::get<1>(i) >= actual_cutoff) {
			period = std::get<0>(i);
			break;
		}
	}

	double pitch_estimate = (sample_rate / period);
	return (pitch_estimate > MPM_LOWER_PITCH_CUTOFF) ? pitch_estimate : -1;
}
