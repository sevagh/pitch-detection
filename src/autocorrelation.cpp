#include "pitch_detection.h"
#include <algorithm>
#include <complex>
#include <ffts/ffts.h>
#include <numeric>
#include <vector>

void
util::acorr_r(const std::vector<float> &audio_buffer, pitch_alloc::BaseAlloc *ba)
{
	if (audio_buffer.size() == 0)
		throw std::invalid_argument("audio_buffer shouldn't be empty");

	std::transform(audio_buffer.begin(), audio_buffer.begin() + ba->nfft,
	    ba->out_im.begin(), [](float x) -> std::complex<float> {
		    return std::complex(x, static_cast<float>(0.0));
	    });

	ffts_execute(ba->fft_forward, ba->out_im.data(), ba->out_im.data());

	std::complex<float> scale = {
	    static_cast<float>(1.0f) / static_cast<float>(ba->nfft), static_cast<float>(0.0)};
	for (int i = 0; i < ba->nfft; ++i)
		ba->out_im[i] *= std::conj(ba->out_im[i]) * scale;

	ffts_execute(ba->fft_backward, ba->out_im.data(), ba->out_im.data());

	std::transform(ba->out_im.begin(), ba->out_im.begin() + ba->nfft,
	    ba->out_real.begin(),
	    [](std::complex<float> cplx) -> float { return std::real(cplx); });
}
