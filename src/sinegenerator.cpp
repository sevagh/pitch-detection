#include <stdlib.h>
#include <vector>
#include "sinegenerator.h"
#include "math.h"
#include "pitch_detector.h"

#define SIZE 4096

void validate_pitch(std::string algo, double frequency, int sample_rate)
{
	double inv_sample_rate = 1.0 / sample_rate;

	double *tone_dual_channel = new double[SIZE/2];
	double *tone_single_channel = new double[SIZE/4];

	int SINE_SIZE = SIZE / 2;
	int LUT_SIZE = SIZE / 4;

	double doublef = (double) frequency;
	double delta_phi = doublef * inv_sample_rate * LUT_SIZE;
	double phase = 0.0;

	int phase_i;
	int i, j;

	int *sine = new int[SINE_SIZE];
	int *LUT = new int[LUT_SIZE];

	for (i = 0; i < LUT_SIZE; i++) {
		LUT[i] = (int) roundf(0x7FFF * sinf(2.0 * M_PI * i / LUT_SIZE));
	}

	for (j = 0; j < SINE_SIZE; j++) {
		phase_i = (int) phase;
		sine[j] = LUT[phase_i];
		phase += delta_phi;
		if (phase >= LUT_SIZE)
			phase -= LUT_SIZE;
	}

	for (j = 0; j < SIZE; j += 2) {
		tone_dual_channel[j] = sine[j / 2];
		tone_dual_channel[j + 1] = tone_dual_channel[j];
		tone_single_channel[j / 2] = sine[j / 2];
	}

	//double pitch = get_pitch(algo, tone_single_channel, sample_rate);
	double pitch = 0.0;
	printf("Sinewave freq: %f\tpitch: %f\n", frequency, pitch);

	delete[] sine;
	delete[] LUT;
}
