#include "pitch_detector.h"

class dft: public PitchDetector
{

private:
	int sample_rate;
	int size;

public:
	dft(int size, int sample_rate);
    ~dft() {};
	double get_pitch(double *data);
};
