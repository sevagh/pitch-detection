#include <iostream>
#include <cstdlib>
#include <vector>
#include "pitch.hpp"
#include "mpm.hpp"
#include "goertzel.hpp"
#include "dft.hpp"
#include "yin.hpp"

#ifdef FFTW_ENABLED
#include "autocorrelation.hpp"
#endif


double get_pitch(std::string algo, std::vector<double> data, int sample_rate)
{
	if (algo == "mpm") {
		return get_pitch_mpm(data, sample_rate);
	} else if (algo == "goertzel") {
		return get_pitch_goertzel(data, sample_rate);
	} else if (algo == "dft") {
		return get_pitch_dft(data, sample_rate);
	} else if (algo == "autocorrelation") {
#ifdef FFTW_ENABLED
		return get_pitch_autocorrelation(data, sample_rate);
#else
		std::cout << "Can't use autocorrelation without FFTW\n";
		std::exit(-1);
#endif
	} else if (algo == "yin") {
		return get_pitch_yin(data, sample_rate);
	} else {
		std::cout << algo << " is not a valid algo\n";
		std::exit(-1);
	}
}
