#include "pitch_detection_priv.h"
#include <algorithm>
#include <complex>
#include <ffts/ffts.h>
#include <numeric>
#include <vector>

void
acorr_r(const std::vector<double> &audio_buffer, pitch_alloc::Mpm *ma)
{
	if (audio_buffer.size() == 0)
		throw std::invalid_argument("audio_buffer shouldn't be empty");

	std::transform(audio_buffer.begin(), audio_buffer.begin() + ma->N,
	    ma->out_im.begin(),
	    [](double x) -> std::complex<double> { return std::complex(x, 0.0); });

	ffts_execute(ma->fft_forward, ma->out_im.data(), ma->out_im.data());

	std::complex<float> scale = {1.0f / (float)(ma->N * 2), 0.0};
	for (int i = 0; i < ma->N; ++i)
		ma->out_im[i] *= std::conj(ma->out_im[i]) * scale;

	ffts_execute(ma->fft_backward, ma->out_im.data(), ma->out_im.data());

	std::transform(ma->out_im.begin(), ma->out_im.begin() + ma->N,
	    ma->out_real.begin(),
	    [](std::complex<float> cplx) -> double { return std::real(cplx); });
}
