//
// Created by sevag on 4/27/15.
//

#ifndef MPM_MPM_H
#define MPM_MPM_H

class mpm {

private:
    double sampling_rate;
    int data_size;
    int fft_flag;

    void normalized_square_difference_time_domain(double *audio_buffer);
    void normalized_square_difference_fft(double *audio_buffer);
    void parabolic_interpolation(int tau);
    void peak_picking();

    mpm() { }

public:
    mpm(double sampling_rate, int size, int fft_flag);

    double get_pitch(double *data);
    void cleanup();
};

#endif //MPM_MPM_H

