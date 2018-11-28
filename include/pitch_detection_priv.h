#ifndef PITCH_DETECTION_PRIV_H
#define PITCH_DETECTION_PRIV_H

/* MPM configs */
#define MPM_CUTOFF 0.93
#define MPM_SMALL_CUTOFF 0.5
#define MPM_LOWER_PITCH_CUTOFF 80

/* YIN configs */
#define YIN_DEFAULT_THRESHOLD 0.20

#include <complex>
#include <ffts/ffts.h>
#include <vector>

// forward decl
class PitchAlloc;

std::pair<double, double>
parabolic_interpolation(const std::vector<double> &, double);

void
acorr_r(const std::vector<double> &, PitchAlloc *);

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

#endif /* PITCH_DETECTION__PRIV_H */
