#include "pitch_detector.h"

class autocorrelation: public PitchDetector
{

private:
	double sampling_rate;
	int data_size;

	double get_acf_periodicity(double *data, int size);
	double *get_normalized_acf(double *data);

public:
	autocorrelation();

	void init(double sampling_rate, int size);
	void cleanup();

	double get_pitch(double *data);

};
