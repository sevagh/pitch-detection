//
// Created by sevag on 10/19/16.
//

#ifndef PITCH_DETECTOR_H
#define PITCH_DETECTOR_H

class PitchDetector
{

public:
	virtual double get_pitch(double *data) = 0;
	virtual void cleanup() = 0;
	virtual void resize(double new_sampling_rate, int newsize) = 0;
};

#endif //PITCH_DETECTOR_H
