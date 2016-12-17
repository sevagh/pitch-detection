#include "pitch_detector.h"

class goertzel: public PitchDetector
{

private:
	int sample_rate;
	int size;

public:
	goertzel(int size, int sample_rate);
    ~goertzel() {};

	double get_pitch(double *data);
};
