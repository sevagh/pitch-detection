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

    std::copy(audio_buffer.begin(), audio_buffer.begin() + audio_buffer.size(), ba->out_real.begin());

    ffts_execute(ba->fft_forward, ba->out_real.data(), ba->out_im.data());

    float scale = 1.0f / static_cast<float>(ba->nfft);
    for (int i = 0; i < ba->nfft / 2 + 1; ++i) {
        float re = ba->out_im[i * 2];
        float im = ba->out_im[i * 2 + 1];
        ba->out_im[i * 2] = (re * re - im * im) * scale;
        ba->out_im[i * 2 + 1] = 0.0f; // set imaginary part to 0
    }

    ffts_execute(ba->fft_backward, ba->out_im.data(), ba->out_real.data());
}
