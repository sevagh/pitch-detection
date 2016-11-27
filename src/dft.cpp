#include <math.h>
#include "helper.h"
#include "dft.h"

void dft::init(double sampling_rate, int size)
{
	dft::sampling_rate = sampling_rate;
	dft::data_size = size;
}

static double dft_energy(double frequency, double *arr, int N, double sampling_rate)
{
	double real = 0.0;
	double im = 0.0;
	double E = 0.0;

	double doubleN = (double) N;

	double DFT_k = (frequency*doubleN*(double) (1.0/sampling_rate));

	int index;
	for (index = 0; index < N; index++) {
		double ind = (double) index;
		double real_c = cos(2.0*M_PI*ind*DFT_k/doubleN);
		double im_c = sin(2.0*M_PI*ind*DFT_k/doubleN);
		real += (double) arr[index]*real_c;
		im -= (double) arr[index]*im_c;
	}

	E += real*real + im*im;

	return E/(doubleN*0.5);
}

double dft::get_pitch(double *data)
{
	return looper(data, data_size, sampling_rate, dft_energy);
}
