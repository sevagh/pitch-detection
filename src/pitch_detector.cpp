#include <iostream>
#include <cstdlib>
#include <vector>
#include "pitch_detector.h"
#include "mpm.h"
#include "goertzel.h"
#include "dft.h"
/*
#include "yin.h"


#ifdef FFTW_ENABLED
#include "autocorrelation.h"
#endif
*/

double get_pitch(std::string algo, std::vector<double> data, int sample_rate)
{
	if (algo == "mpm") {
		return get_pitch_mpm(data, sample_rate);
	} else if (algo == "goertzel") {
		return get_pitch_goertzel(data, sample_rate);
	}

	/*
	else if (algo == "dft") {
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
	*/
	return 0.0;
}
