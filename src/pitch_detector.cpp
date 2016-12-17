#include <iostream>
#include <cstdlib>
#include "pitch_detector.h"
#include "mpm.h"
#include "goertzel.h"
#include "dft.h"
#include "yin.h"

#ifdef FFTW_ENABLED
#include "autocorrelation.h"
#endif

PitchDetector *get_pitch_detector(std::string algo, int size, int sample_rate)
{
	std::cout << "Instantiating algorithm: " << algo << std::endl;
	if (algo == "mpm") {
		return new mpm(size, sample_rate);
	} else if (algo == "goertzel") {
		return new goertzel(size, sample_rate);
	} else if (algo == "dft") {
		return new dft(size, sample_rate);
	} else if (algo == "autocorrelation") {
#ifdef FFTW_ENABLED
		return new autocorrelation(size, sample_rate);
#else
		std::cout << "Can't use autocorrelation without FFTW\n";
		std::exit(-1);
#endif
	} else if (algo == "yin") {
		return new yin(size, sample_rate);
	} else {
		std::cout << algo << " is not a valid algo\n";
		std::exit(-1);
	}
}
