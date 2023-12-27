#include "pitch_detection.h"
#include <algorithm>
#include <complex>
#include <ffts/ffts.h>
#include <numeric>
#include <vector>

void
util::acorr_r(
    const std::vector<float> &audio_buffer, pitch_alloc::BaseAlloc *ba)
{
	if (audio_buffer.size() == 0)
		throw std::invalid_argument("audio_buffer shouldn't be empty");

	if (ba->fft_type == pitch_alloc::BaseAlloc::REAL_TO_COMPLEX) {
		// For real-to-complex FFT, fill the real input directly
		std::copy(audio_buffer.begin(), audio_buffer.begin() + ba->nfft,
		    ba->out_real.begin());
		ffts_execute(ba->fft_forward, ba->out_real.data(), ba->out_im.data());
	} else {
		// For complex-to-complex FFT, fill the complex input
		std::transform(audio_buffer.begin(), audio_buffer.begin() + ba->nfft,
		    ba->out_im.begin(), [](float x) -> std::complex<float> {
			    return std::complex<float>(x, 0.0f);
		    });
		ffts_execute(ba->fft_forward, ba->out_im.data(), ba->out_im.data());
	}

	// Apply autocorrelation and scaling
	std::complex<float> scale = 1.0f / static_cast<float>(ba->nfft);
	int output_size = (ba->fft_type == pitch_alloc::BaseAlloc::REAL_TO_COMPLEX)
	                      ? ba->nfft / 2 + 1
	                      : ba->nfft;
	for (int i = 0; i < output_size; ++i) {
		ba->out_im[i] *= std::conj(ba->out_im[i]) * scale;
	}

	// Execute backward FFT
	if (ba->fft_type == pitch_alloc::BaseAlloc::REAL_TO_COMPLEX) {
		ffts_execute(ba->fft_backward, ba->out_im.data(), ba->out_real.data());
		// Convert back to real
		std::copy(ba->out_real.begin(), ba->out_real.begin() + ba->nfft,
		    ba->out_real.begin());
	} else {
		ffts_execute(ba->fft_backward, ba->out_im.data(), ba->out_im.data());
		// Extract the real part
		std::transform(ba->out_im.begin(), ba->out_im.begin() + ba->nfft,
		    ba->out_real.begin(), [](const std::complex<float> &cplx) -> float {
			    return std::real(cplx);
		    });
	}
}
