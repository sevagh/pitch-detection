#include <iostream>

#ifndef PITCH_DETECTOR_H
#define PITCH_DETECTOR_H

class PitchDetector
{

public:
	virtual void init(double sampling_rate, int size) = 0;
	virtual void cleanup() = 0;

	virtual double get_pitch(double *data) = 0;

	virtual ~PitchDetector() {};
};

PitchDetector *get_pitch_detector(std::string pitch_detector_type);

#endif /* PITCH_DETECTOR_H */
