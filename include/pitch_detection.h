#ifndef PITCH_DETECTION_H
#define PITCH_DETECTION_H

#include <complex>
#include <ffts/ffts.h>
#include <mlpack/core.hpp>
#include <mlpack/methods/hmm/hmm.hpp>
#include <stdexcept>
#include <vector>

/* ignore me plz */
namespace detail
{
template <typename T>
std::vector<size_t>
bin_pitches(const std::vector<std::pair<T, T>>);

mlpack::hmm::HMM<mlpack::distribution::DiscreteDistribution>
build_hmm();

void
init_pitch_bins();
} // namespace detail

/*
 * The pitch namespace contains the functions:
 *
 * 	pitch::mpm(data, sample_rate)
 * 	pitch::yin(data, sample_rate)
 * 	pitch::pyin(data, sample_rate)
 * 	pitch::pmpm(data, sample_rate)
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
T
pyin(const std::vector<T> &, int);

template <typename T>
T
pmpm(const std::vector<T> &, int);
} // namespace pitch

/*
 * This namespace is useful for repeated calls to pitch for the same size of
 * buffer.
 *
 * It contains the classes Yin and Mpm which contain the allocated buffers
 * and each implement a `pitch(data, sample_rate)` and
 * `probablistic_pitch(data, sample_rate)` method.
 */
namespace pitch_alloc
{

template <typename T> class BaseAlloc
{
  public:
	long nfft;
	std::vector<std::complex<T>> out_im;
	std::vector<T> out_real;
	ffts_plan_t *fft_forward;
	ffts_plan_t *fft_backward;
	mlpack::hmm::HMM<mlpack::distribution::DiscreteDistribution> hmm;

	BaseAlloc(long nfft)
	    : nfft(nfft), out_im(std::vector<std::complex<T>>(nfft)),
	      out_real(std::vector<T>(nfft))
	{
		if (nfft == 0) {
			throw std::bad_alloc();
		}

		fft_forward = ffts_init_1d(nfft, FFTS_FORWARD);
		fft_backward = ffts_init_1d(nfft, FFTS_BACKWARD);
		detail::init_pitch_bins();
		hmm = detail::build_hmm();
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
	Mpm(long nfft) : BaseAlloc<T>(nfft){};

	T
	pitch(const std::vector<T> &, int);

	T
	probabilistic_pitch(const std::vector<T> &, int);
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
	int yin_buffer_size;
	std::vector<T> yin_buffer;

	Yin(long nfft)
	    : BaseAlloc<T>(nfft),
		  yin_buffer_size(nfft / 2),
	      yin_buffer(std::vector<T>(nfft))
	{
		if (yin_buffer_size == 0) {
			throw std::bad_alloc();
		}
	}

	T
	pitch(const std::vector<T> &, int);

	T
	probabilistic_pitch(const std::vector<T> &, int);
};
} // namespace pitch_alloc

namespace util
{
template <typename T>
std::pair<T, T>
parabolic_interpolation(const std::vector<T> &, int);

template <typename T>
void
acorr_r(const std::vector<T> &, pitch_alloc::BaseAlloc<T> *);

template <typename T>
T
pitch_from_hmm(mlpack::hmm::HMM<mlpack::distribution::DiscreteDistribution>,
    const std::vector<std::pair<T, T>>);
} // namespace util

#endif /* PITCH_DETECTION_H */
