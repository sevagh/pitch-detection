#ifndef PITCH_DETECTION_H
#define PITCH_DETECTION_H

#include <complex>
#include <ffts/ffts.h>
#include <stdexcept>
#include <vector>

/*
 * Allocate the buffers for Autocorrelation for re-use.
 * Intended for multiple consistently-sized audio buffers.
 *
 * Usage: PitchAlloc pa(1024)
 *
 * It will throw std::bad_alloc for invalid sizes (<1)
 */
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
		if (N == 0) {
			throw std::bad_alloc();
		}

		fft_forward = ffts_init_1d(N2, FFTS_FORWARD);
		fft_backward = ffts_init_1d(N2, FFTS_BACKWARD);
	}

	~PitchAlloc()
	{
		ffts_free(fft_forward);
		ffts_free(fft_backward);
	}

	void
	clear()
	{
		std::fill(out_im.begin(), out_im.end(), std::complex<double>(0.0, 0.0));
	}
};

/*
 * Allocate the buffers for MPM for re-use.
 * Intended for multiple consistently-sized audio buffers.
 *
 * Usage: MpmAlloc ma(1024)
 *
 * It will throw std::bad_alloc for invalid sizes (<1)
 */
class MpmAlloc : public PitchAlloc
{
  public:
	MpmAlloc(long audio_buffer_size) : PitchAlloc(audio_buffer_size)
	{
	}
};

/*
 * Allocate the buffers for YIN for re-use.
 * Intended for multiple consistently-sized audio buffers.
 *
 * Usage: YinAlloc ya(1024)
 *
 * It will throw std::bad_alloc for invalid sizes (<2)
 */
class YinAlloc : public PitchAlloc
{
  public:
	long N4;
	std::vector<double> yin_buffer;

	YinAlloc(long audio_buffer_size)
	    : PitchAlloc(audio_buffer_size), N4(N / 2),
	      yin_buffer(std::vector<double>(N4))
	{
		if (N4 == 0) {
			throw std::bad_alloc();
		}
	}
};

/*
 * The pitch namespace contains the functions:
 *
 * 	autocorrelation(data, sample_rate)
 * 	mpm(data, sample_rate)
 * 	yin(data, sample_rate)
 *
 * It will auto-allocate any buffers.
 */
namespace pitch
{

double
autocorrelation(const std::vector<double> &, int);

double
yin(const std::vector<double> &, int);

double
mpm(const std::vector<double> &, int);

} // namespace pitch

/*
 * The pitch_manual_alloc namespace contains the functions:
 *
 * 	autocorrelation(data, sample_rate, PitchAlloc)
 * 	mpm(data, sample_rate, MpmAlloc)
 * 	yin(data, sample_rate, YinAlloc)
 *
 * These ones are Bring-Your-Own-Alloc for reusing buffers.
 */
namespace pitch_manual_alloc
{

double
autocorrelation(const std::vector<double> &, int, PitchAlloc *);

double
yin(const std::vector<double> &, int, YinAlloc *);

double
mpm(const std::vector<double> &, int, MpmAlloc *);

} // namespace pitch_manual_alloc

#endif /* PITCH_DETECTION_H */
