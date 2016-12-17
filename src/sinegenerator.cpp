#include <stdlib.h>
#include "sinegenerator.h"
#include "math.h"
#include "pitch_detector.h"

#define SIZE 4096

sinegenerator::sinegenerator(double sample_rate, double frequency)
{
	sinegenerator::sample_rate = sample_rate;
	sinegenerator::inv_sample_rate = 1.0 / sinegenerator::sample_rate;

	sinegenerator::frequency = frequency;

	sinegenerator::tone_dual_channel = new double[SIZE];
	sinegenerator::size_dual_channel = SIZE;

	sinegenerator::tone_single_channel = new double[SIZE / 2];
	sinegenerator::size_single_channel = SIZE / 2;

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

	delete[] sine;
	delete[] LUT;
}

void sinegenerator::validate_pitch(std::string algo)
{
	PitchDetector *pitch_detector = get_pitch_detector(algo, size_single_channel, sample_rate);
	double pitch = pitch_detector->get_pitch(tone_single_channel);
	printf("Sinewave freq: %f\tpitch: %f\n", frequency, pitch);
}

sinegenerator::~sinegenerator()
{
	delete[] tone_single_channel;
	delete[] tone_dual_channel;
}
