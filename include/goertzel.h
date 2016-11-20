#include "pitch_detector.h"

class goertzel: public PitchDetector
{

private:
	double sampling_rate;
	int data_size;

public:
	goertzel() {};

	void init(double sampling_rate, int size);
	void cleanup() {};

	double get_pitch(double *data);
};
