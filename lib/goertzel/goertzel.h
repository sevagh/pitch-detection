//
// Created by sevag on 4/28/15.
//

#ifndef MPM_GOERTZEL_H
#define MPM_GOERTZEL_H

#include "../pitch_detector/pitch_detector.h"

class goertzel: public PitchDetector {

private:
    double sampling_rate;
    int data_size;

    goertzel() { }

    double goertzel_energy(int frequency, double *arr, int N);
    float get_snr(int frequency, double *arr, int N);

public:
    goertzel(double sampling_rate, int size);

    double get_pitch(double *data);

    void cleanup();
};

#endif //MPM_GOERTZEL_H
