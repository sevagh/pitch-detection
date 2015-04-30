//
// Created by sevag on 4/27/15.
//

#ifndef MPM_MPM_H
#define MPM_MPM_H

class mpm {

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

    mpm() { }

public:
    mpm(double sampling_rate, int size);

    double get_pitch(double *data);
    void cleanup();

};

#endif //MPM_MPM_H

