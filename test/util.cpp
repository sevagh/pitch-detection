#include "util.h"
#include "pitch_detection.h"
#include <cmath>
#include <float.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// thanks to Pizer's excellent implementation
// https://pizer.wordpress.com/2010/02/08/fast-digital-sine-oscillator/
class sine_generator
{
    std::complex<double> phase;
    std::complex<double> rotation;

    template<typename Sink>
    void internal_generate(int count, Sink sink);

public:
    sine_generator() : phase(1),rotation(1) {}

    void set_frequency(double rad)
    { rotation = std::polar(double(1),rad); }

    void reset_phase(double rad)
    { phase = std::polar(double(1),rad); }

    template<typename OutputIter>
    void generate(int count, OutputIter begin)
    {
        internal_generate(count,[&](double smp){
            *begin = smp; ++begin;
        });
    }

    template<typename ForwardIter, typename BinOp>
    void generate(int count, ForwardIter begin, BinOp binop)
    {
        internal_generate(count,[&](double smp){
           *begin = binop(*begin,smp); ++begin;
        });
    }
};

template<typename Sink>
void sine_generator::internal_generate(int count, Sink sink)
{
    std::complex<double> rot_forx = pow(rotation,12);
    phase /= abs(phase); // prevent drifting off due to rounding errors
    // use linear recurrence for generating the samples
    double const fy = 2 * real(rotation);
    double const fx = 2 * real(rot_forx);
    double y0 = imag(phase);
    double x0 = real(phase);
    double y1 = imag(phase*conj(rotation));
    double x1 = real(phase*conj(rot_forx));
    double tt; // temporary variable
    while (count >= 12) {
        // 4*3 = 12 samples in a row ...
        for (int k=0; k<4; ++k) {
            // output       | compute next sample
            sink(y0); tt = fy*y0-y1;
            sink(tt); y1 = fy*tt-y0;
            sink(y1); y0 = fy*y1-tt;
        }
        // update x as well ...
        tt = fx*x0-x1; x1 = x0; x0 = tt;
        count -= 12;
    }
    phase = std::complex<double>(x0,y0);
    while (count-- > 0) {
        sink(imag(phase));
        phase *= rotation;
    }
}

std::vector<double> test_util::sinewave(
    size_t size, double frequency, int sample_rate) {
    std::vector<double> buffer(size);

    sine_generator sg;

    sg.set_frequency(frequency * 2 * M_PI / sample_rate);
    sg.reset_phase(0);

    sg.generate(size, buffer.begin());

    return buffer;
}

std::vector<double>
test_util::vec_from_file(std::string path)
{
	std::vector<double> data;
	std::ifstream infile(path);

	if (infile.fail()) {
		std::cerr << "File '" << path << "' doesn't exist, exiting"
		          << std::endl;
		exit(1);
	}

	double val;
	while (infile >> val)
		data.push_back(val);

	return data;
}
