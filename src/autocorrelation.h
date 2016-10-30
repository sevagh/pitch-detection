//
// Created by sevag on 4/28/15.
//

#ifndef MPM_AUTOCORRELATION_H
#define MPM_AUTOCORRELATION_H

#include "pitch_detector.h"

class autocorrelation: public PitchDetector
{

private:
	double sampling_rate;
	int data_size;

	autocorrelation() { }

	double get_acf_periodicity(double *data, int size);
	double *get_normalized_acf(double *data);

public:
	autocorrelation(double sampling_rate, int size);

	double get_pitch(double *data);

	void cleanup();
	void resize(double new_sampling_rate, int newsize);
};


#endif //MPM_AUTOCORRELATION_H
