#include "pitch_detector.h"

class dft: public PitchDetector
{

private:
	double sampling_rate;
	int data_size;

public:
	dft() {};

	void init(double sampling_rate, int size);
	void cleanup() {};

	double get_pitch(double *data);
};
