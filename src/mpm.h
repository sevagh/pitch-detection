#include "pitch_detector.h"
#include <vector>

#define CUTOFF 0.93 //0.97 is default
#define SMALL_CUTOFF 0.5
#define LOWER_PITCH_CUTOFF 80 //hz

double get_pitch_mpm(std::vector<double> data, int sample_rate);
