//
// Created by sevag on 4/27/15.
//

#ifndef MPM_MPM_H
#define MPM_MPM_H

class mpm {

private:
    double sampling_rate;
    int data_size;

    void normalized_square_difference(double *audio_buffer);
    void parabolic_interpolation(int tau);
    void peak_picking();

    mpm() { }

public:
    mpm(double sampling_rate, int size);

    double get_pitch(double *data);
    void cleanup();
};

#endif //MPM_MPM_H

