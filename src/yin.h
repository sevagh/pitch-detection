#include "pitch_detector.h"

class yin: public PitchDetector
{

private:
	int sample_rate;
	int size;
	int yin_buffer_size;
	double *yin_buffer;

	void difference(double *data);
	void cumulative_mean_normalized_difference();
	int absolute_threshold();
	double parabolic_interpolation(int tau_estimate);

public:
	yin(int size, int sample_rate);
    ~yin();

	double get_pitch(double *data);
};
