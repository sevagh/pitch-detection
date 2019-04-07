#ifndef PITCH_DETECTION_H
#define PITCH_DETECTION_H

#include <complex>
#include <ffts/ffts.h>
#include <stdexcept>
#include <vector>

/*
 * The pitch namespace contains the functions:
 *
 * 	pitch::mpm(data, sample_rate)
 * 	pitch::yin(data, sample_rate)
 * 	pitch::pyin(data, sample_rate)
 *
 * It will auto-allocate any buffers.
 */
namespace pitch
{

template <typename T>
T
yin(const std::vector<T> &, int);

template <typename T>
T
mpm(const std::vector<T> &, int);

template <typename T>
T
swipe(const std::vector<T> &, int);

/*
 * pyin and pmpm emit pairs of pitch/probability
 */
template <typename T>
std::vector<std::pair<T, T>>
pyin(const std::vector<T> &, int);

template <typename T>
std::vector<std::pair<T, T>>
pmpm(const std::vector<T> &, int);
} // namespace pitch

/*
 * This namespace is useful for repeated calls to pitch for the same size of
 * buffer.
 *
 * It contains the classes Yin and Mpm which contain the allocated buffers
 * and each implement a `pitch(data, sample_rate)` and
 * `probablistic_pitches(data, sample_rate)` method.
 */
namespace pitch_alloc
{

template <typename T> class BaseAlloc
{
  public:
	long N;
	std::vector<std::complex<float>> out_im;
	std::vector<T> out_real;
	ffts_plan_t *fft_forward;
	ffts_plan_t *fft_backward;

	BaseAlloc(long audio_buffer_size)
	    : N(audio_buffer_size), out_im(std::vector<std::complex<float>>(N * 2)),
	      out_real(std::vector<T>(N))
	{
		if (N == 0) {
			throw std::bad_alloc();
		}

		fft_forward = ffts_init_1d(N * 2, FFTS_FORWARD);
		fft_backward = ffts_init_1d(N * 2, FFTS_BACKWARD);
	}

	~BaseAlloc()
	{
		ffts_free(fft_forward);
		ffts_free(fft_backward);
	}

  protected:
	void
	clear()
	{
		std::fill(out_im.begin(), out_im.end(), std::complex<T>(0.0, 0.0));
	}
};

/*
 * Allocate the buffers for MPM for re-use.
 * Intended for multiple consistently-sized audio buffers.
 *
 * Usage: pitch_alloc::Mpm ma(1024)
 *
 * It will throw std::bad_alloc for invalid sizes (<1)
 */
template <typename T> class Mpm : public BaseAlloc<T>
{
  public:
	Mpm(long audio_buffer_size) : BaseAlloc<T>(audio_buffer_size){};

	T
	pitch(const std::vector<T> &, int);

	std::vector<std::pair<T, T>>
	probabilistic_pitches(const std::vector<T> &, int);
};

/*
 * Allocate the buffers for YIN for re-use.
 * Intended for multiple consistently-sized audio buffers.
 *
 * Usage: pitch_alloc::Yin ya(1024)
 *
 * It will throw std::bad_alloc for invalid sizes (<2)
 */
template <typename T> class Yin : public BaseAlloc<T>
{
  public:
	std::vector<T> yin_buffer;

	Yin(long audio_buffer_size)
	    : BaseAlloc<T>(audio_buffer_size),
	      yin_buffer(std::vector<T>(audio_buffer_size / 2))
	{
		if (audio_buffer_size / 2 == 0) {
			throw std::bad_alloc();
		}
	}

	T
	pitch(const std::vector<T> &, int);

	std::vector<std::pair<T, T>>
	probabilistic_pitches(const std::vector<T> &, int);
};
} // namespace pitch_alloc

#endif /* PITCH_DETECTION_H */
