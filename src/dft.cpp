#include "pitch_detection.h"
#include "pitch_detection_priv.h"
#include <vector>

static double
dft_energy(
    double frequency, const std::vector<double> &data, double sample_rate)
{
	double real = 0.0;
	double im = 0.0;
	double E = 0.0;

	double doubleN = (double)data.size();

	double DFT_k = (frequency * doubleN * (double)(1.0 / sample_rate));

	for (uint index = 0; index < data.size(); index++) {
		double ind = (double)index;
		double real_c = cos(2.0 * M_PI * ind * DFT_k / doubleN);
		double im_c = sin(2.0 * M_PI * ind * DFT_k / doubleN);
		real += (double)data[index] * real_c;
		im -= (double)data[index] * im_c;
	}

	E += real * real + im * im;

	return E / (doubleN * 0.5);
}

double
get_pitch_dft(const std::vector<double> &data, int sample_rate)
{
	return looper(data, sample_rate, dft_energy);
}
