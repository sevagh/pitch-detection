#ifndef PITCH_DETECTOR_H
#define PITCH_DETECTOR_H

class PitchDetector
{

public:
	virtual void init(double sampling_rate, int size) = 0;
	virtual void cleanup() = 0;

	virtual double get_pitch(double *data) = 0;
};

#endif /* PITCH_DETECTOR_H */
