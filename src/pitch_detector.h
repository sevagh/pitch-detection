#include <iostream>
#include <vector>

#ifndef PITCH_DETECTOR_H
#define PITCH_DETECTOR_H

class PitchDetector
{
public:
    virtual ~PitchDetector() {};
    virtual double get_pitch(double *data) = 0;
};

double get_pitch(std::string algo, std::vector<double> data, int sample_rate);

#endif /* PITCH_DETECTOR_H */
