#include "pitch_detector.h"

class yin: public PitchDetector
{

private:
	double sampling_rate;
	int data_size;
	int yin_buffer_size;
	double *yin_buffer;

	void difference(double *data);
	void cumulative_mean_normalized_difference();
	int absolute_threshold();
	double parabolic_interpolation(int tau_estimate);

public:
	yin();

	void init(double sampling_rate, int size);
	void cleanup();

	double get_pitch(double *data);
};
