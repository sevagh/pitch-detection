#include "pitch_detection.h"
#include "pitch_detection_priv.h"
#include <vector>

static double
goertzel_energy(
    double frequency, const std::vector<double> &data, double sample_rate)
{
	double doubleN = (double)data.size();

	double goertzel_k = (doubleN * frequency * (double)(1.0 / sample_rate));
	double goertzel_w = (2.0 * (double)cos(2.0 * M_PI * goertzel_k / doubleN));
	double goertzel_re = ((double)cos(2.0 * M_PI * goertzel_k / doubleN));
	double goertzel_im = ((double)sin(2.0 * M_PI * goertzel_k / doubleN));

	double Q0 = 0.0;
	double Q1 = 0.0;
	double Q2;

	double re;
	double im;
	double E;

	for (uint index = 0; index < data.size(); ++index) {
		Q2 = Q1;
		Q1 = Q0;
		Q0 = (double)data[index] + goertzel_w * Q1 - Q2;
	}

	re = Q0 - goertzel_re * Q1;
	im = goertzel_im * Q1;
	E = re * re + im * im;

	return E / (doubleN * 0.5);
}

double
get_pitch_goertzel(const std::vector<double> &data, int sample_rate)
{
	return looper(data, sample_rate, goertzel_energy);
}
