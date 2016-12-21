#include <cmath>
#include <vector>
#include "loop.hpp"

static double goertzel_energy(double frequency, std::vector<double> arr, int N, int sample_rate);

inline double get_pitch_goertzel(std::vector<double> data, int sample_rate)
{
	return loop(data, data.size(), sample_rate, goertzel_energy);
}

static double goertzel_energy(double frequency, std::vector<double> arr, int N, int sample_rate)
{
	double doubleN = (double) N;

	double goertzel_k = (doubleN * frequency * (double) (1.0 / (double) sample_rate));
	double goertzel_w = (2.0 * (double) cos(2.0 * M_PI * goertzel_k / doubleN));
	double goertzel_re = ((double) cos(2.0 * M_PI * goertzel_k / doubleN));
	double goertzel_im = ((double) sin(2.0 * M_PI * goertzel_k / doubleN));

	double Q0 = 0.0;
	double Q1 = 0.0;
	double Q2;

	double re;
	double im;
	double E;

	int index;

	for (index = 0; index < N; index++) {
		Q2 = Q1;
		Q1 = Q0;
		Q0 = (double) arr[index] + goertzel_w * Q1 - Q2;
	}

	re = Q0 - goertzel_re * Q1;
	im = goertzel_im * Q1;
	E = re * re + im * im;

	return E / (doubleN * 0.5);
}
