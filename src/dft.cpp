#include <math.h>
#include "helper.h"
#include "dft.h"

dft::dft()
{
}

void dft::init(double sampling_rate, int size)
{
	dft::sampling_rate = sampling_rate;
	dft::data_size = size;
}

double dft_energy(double frequency, double *arr, int N, double sampling_rate)
{
	float real = 0.0f;
	float im = 0.0f;
	float E = 0.0f;

	float floatN = (float) N;

	float DFT_k = (frequency*floatN*(float) (1.0/sampling_rate));

	int index;
	for (index = 0; index < N; index++) {
		float ind = (float) index;
		float real_c = cos(2.0f*M_PI*ind*DFT_k/floatN);
		float im_c = sin(2.0f*M_PI*ind*DFT_k/floatN);
		real += (float) arr[index]*real_c;
		im -= (float) arr[index]*im_c;
	}

	E += real*real + im*im;

	return E/(floatN*0.5);
}

double dft::get_pitch(double *data)
{
	return looper(data, data_size, sampling_rate, dft_energy);
}

void dft::cleanup()
{
}
