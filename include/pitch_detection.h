#ifndef PITCH_DETECTION_H
#define PITCH_DETECTION_H

#include <complex>
#include <ffts/ffts.h>
#include <vector>

class PitchAlloc
{
  public:
	long N, N2;
	std::vector<std::complex<float>> out_im;
	std::vector<double> out_real;
	ffts_plan_t *fft_forward;
	ffts_plan_t *fft_backward;

	PitchAlloc(long audio_buffer_size)
	    : N(audio_buffer_size), N2(2 * N),
	      out_im(std::vector<std::complex<float>>(N2)),
	      out_real(std::vector<double>(N))
	{
		fft_forward = ffts_init_1d(N2, FFTS_FORWARD);
		fft_backward = ffts_init_1d(N2, FFTS_BACKWARD);
	}

	~PitchAlloc()
	{
		ffts_free(fft_forward);
		ffts_free(fft_backward);
	}
};

class YinAlloc : public PitchAlloc
{
  public:
	long N4;
	std::vector<double> yin_buffer;

	YinAlloc(long audio_buffer_size)
	    : PitchAlloc(audio_buffer_size), N4(N / 2),
	      yin_buffer(std::vector<double>(N4))
	{
	}
};

namespace pitch
{
double
autocorrelation(const std::vector<double> &, int);

double
yin(const std::vector<double> &, int);

double
mpm(const std::vector<double> &, int);
} // namespace pitch

namespace pitch_manual_alloc
{
double
autocorrelation(const std::vector<double> &, int, PitchAlloc *);

double
yin(const std::vector<double> &, int, YinAlloc *);

double
mpm(const std::vector<double> &, int, PitchAlloc *);
} // namespace pitch_manual_alloc

#endif /* PITCH_DETECTION_H */
