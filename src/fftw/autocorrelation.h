#include "pitch_detector.h"

class autocorrelation: public PitchDetector
{

private:
	int sample_rate;
	int size;

	double get_acf_periodicity(double *data, int size);

public:
	autocorrelation(int size, int sample_rate);
    ~autocorrelation() {};

	double get_pitch(double *data);
};

double* autocorrelate(double *data, int size);
