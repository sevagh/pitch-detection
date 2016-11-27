#include <iostream>
#include "pitch_detector.h"
#include "mpm.h"
#include "goertzel.h"
#include "dft.h"
#include "yin.h"
#include "testbench.h"

#ifdef FFTW_ENABLED
#include "autocorrelation.h"
#endif

PitchDetector *get_pitch_detector(std::string pitch_detector_type)
{
	PitchDetector *pitch_detector;

	if (pitch_detector_type == "mpm") {
		pitch_detector = new mpm();
	} else if (pitch_detector_type == "goertzel") {
		pitch_detector = new goertzel();
	} else if (pitch_detector_type == "dft") {
		pitch_detector = new dft();
	} else if (pitch_detector_type == "autocorrelation") {
#ifdef FFTW_ENABLED
		pitch_detector = new autocorrelation();
#else
		std::cout << "Can't use autocorrelation without FFTW\n";
		exit(-1);
#endif
	} else if (pitch_detector_type == "yin") {
		pitch_detector = new yin();
	} else {
		std::cout << pitch_detector_type << " is not a valid algo\n";
		exit(-1);
	}
	return pitch_detector;
}
