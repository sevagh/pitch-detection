#include <math.h>
#include "goertzel.h"
#include "helper.h"

goertzel::goertzel(int size, int sample_rate)
{
	goertzel::sample_rate = sample_rate;
	goertzel::size = size;
}

static double goertzel_energy(double frequency, double *arr, int N, double sample_rate)
{
	double doubleN = (double) N;

	double goertzel_k = (doubleN * frequency * (double) (1.0 / sample_rate));
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

double goertzel::get_pitch(double *data)
{
	return looper(data, size, sample_rate, goertzel_energy);
}
