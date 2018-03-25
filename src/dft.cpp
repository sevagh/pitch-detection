#include "pitch_detection.h"
#include "pitch_detection_priv.h"
#include <vector>

static double
dft_energy(double frequency, std::vector<double> &arr, double sample_rate)
{
	double real = 0.0;
	double im = 0.0;
	double E = 0.0;

	double doubleN = (double)arr.size();

	double DFT_k = (frequency * doubleN * (double)(1.0 / sample_rate));

	for (uint index = 0; index < arr.size(); index++) {
		double ind = (double)index;
		double real_c = cos(2.0 * M_PI * ind * DFT_k / doubleN);
		double im_c = sin(2.0 * M_PI * ind * DFT_k / doubleN);
		real += (double)arr[index] * real_c;
		im -= (double)arr[index] * im_c;
	}

	E += real * real + im * im;

	return E / (doubleN * 0.5);
}

double
get_pitch_dft(std::vector<double> &data, int sample_rate)
{
	return looper(data, sample_rate, dft_energy);
}
