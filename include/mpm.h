#include "pitch_detector.h"

#define CUTOFF 0.93 //0.97 is default
#define SMALL_CUTOFF 0.5
#define LOWER_PITCH_CUTOFF 80 //hz

#define MAX(a, b) ((a < b) ?  (b) : (a))

class mpm: public PitchDetector
{

private:
	double sampling_rate;
	int data_size;

	double turning_point_x, turning_point_y;
	double *nsdf;

	int *max_positions;
	double *period_estimates;
	double *amp_estimates;

	int max_positions_ptr, period_estimates_ptr, amp_estimates_ptr;

	void parabolic_interpolation(int tau);
	void peak_picking();
	void nsdf_time_domain(double *data);

public:
	mpm() {};

	void init(double sampling_rate, int size);
	void cleanup();

	double get_pitch(double *data);
};
