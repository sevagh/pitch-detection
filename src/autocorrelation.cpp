#include <algorithm>
#include <complex>
#include <ffts/ffts.h>
#include <numeric>
#include <pitch_detection.h>
#include <pitch_detection_priv.h>
#include <stdexcept>
#include <vector>

void
acorr_r(const std::vector<double> &audio_buffer, PitchAlloc *pa)
{
	if (audio_buffer.size() == 0)
		throw std::invalid_argument("audio_buffer shouldn't be empty");

	std::transform(audio_buffer.begin(), audio_buffer.begin() + pa->N,
	    pa->out_im.begin(),
	    [](double x) -> std::complex<double> { return std::complex(x, 0.0); });

	ffts_execute(pa->fft_forward, pa->out_im.data(), pa->out_im.data());

	std::complex<float> scale = {1.0f / (float)pa->N2, 0.0};
	for (int i = 0; i < pa->N; ++i)
		pa->out_im[i] *= std::conj(pa->out_im[i]) * scale;

	ffts_execute(pa->fft_backward, pa->out_im.data(), pa->out_im.data());

	std::transform(pa->out_im.begin(), pa->out_im.begin() + pa->N,
	    pa->out_real.begin(),
	    [](std::complex<float> cplx) -> double { return std::real(cplx); });
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
pitch::autocorrelation(const std::vector<double> &audio_buffer, int sample_rate)
{
	PitchAlloc pa(audio_buffer.size());

	return pitch_manual_alloc::autocorrelation(audio_buffer, sample_rate, &pa);
}

double
pitch_manual_alloc::autocorrelation(
    const std::vector<double> &audio_buffer, int sample_rate, PitchAlloc *pa)
{
	acorr_r(audio_buffer, pa);

	double peak_bin_index_periodicity = get_acf_periodicity(pa->out_real);

	pa->clear();

	return (sample_rate / peak_bin_index_periodicity);
}
