#include <iostream>
#include <cstdlib>
#include "pitch_detector.h"
#include "mpm.h"
#include "goertzel.h"
#include "dft.h"
#include "yin.h"
#include "testbench.h"

#ifdef FFTW_ENABLED
#include "autocorrelation.h"
#endif

PitchDetector *get_pitch_detector(std::string pitch_detector_type, int size, int sample_rate)
{
	if (pitch_detector_type == "mpm") {
		return new mpm(size, sample_rate);
	} else if (pitch_detector_type == "goertzel") {
        return new goertzel(size, sample_rate);
	} else if (pitch_detector_type == "dft") {
        return new dft(size, sample_rate);
	} else if (pitch_detector_type == "autocorrelation") {
#ifdef FFTW_ENABLED
        return new autocorrelation(size, sample_rate);
#else
		std::cout << "Can't use autocorrelation without FFTW\n";
		std::exit(-1);
#endif
	} else if (pitch_detector_type == "yin") {
        return new yin(size, sample_rate);
	} else {
		std::cout << pitch_detector_type << " is not a valid algo\n";
		std::exit(-1);
	}
}
