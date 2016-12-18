#include "pitch_detector.h"
#include <vector>
#include <tuple>

#define CUTOFF 0.93 //0.97 is default
#define SMALL_CUTOFF 0.5
#define LOWER_PITCH_CUTOFF 80 //hz

#define MAX(a, b) ((a < b) ?  (b) : (a))

class mpm: public PitchDetector
{

private:
    int sample_rate, size;

	std::vector<double> nsdf_time_domain(double *data);

public:
	mpm(int size, int sample_rate);
    ~mpm();

    double get_pitch(double *data);
};
