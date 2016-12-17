#include "pitch_detector.h"

#define CUTOFF 0.93 //0.97 is default
#define SMALL_CUTOFF 0.5
#define LOWER_PITCH_CUTOFF 80 //hz

#define MAX(a, b) ((a < b) ?  (b) : (a))

class mpm: public PitchDetector
{

private:
    int sample_rate, data_size, max_positions_ptr, period_estimates_ptr, amp_estimates_ptr;
	int *max_positions;

	double turning_point_x, turning_point_y;
	double *nsdf, *period_estimates, *amp_estimates;

	void parabolic_interpolation(int tau);
	void peak_picking();
	void nsdf_time_domain(double *data);

public:
	mpm(int size, int sample_rate);
    ~mpm();

    double get_pitch(double *data);
};
