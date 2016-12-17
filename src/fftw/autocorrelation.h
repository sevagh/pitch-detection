#include "pitch_detector.h"

class autocorrelation: public PitchDetector
{

private:
	int sample_rate;
	int size;

	double get_acf_periodicity(double *data, int size);
	double *get_normalized_acf(double *data);

public:
	autocorrelation(int size, int sample_rate);
    ~autocorrelation() {};

	double get_pitch(double *data);
};
