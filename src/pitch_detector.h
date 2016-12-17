#include <iostream>

#ifndef PITCH_DETECTOR_H
#define PITCH_DETECTOR_H

class PitchDetector
{
public:
    virtual ~PitchDetector() {};
    virtual double get_pitch(double *data) = 0;
};

PitchDetector *get_pitch_detector(std::string algo, int size, int sample_rate);

#endif /* PITCH_DETECTOR_H */
