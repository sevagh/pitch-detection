//
// Created by sevag on 4/28/15.
//

#ifndef MPM_AUTOCORRELATION_H
#define MPM_AUTOCORRELATION_H


class autocorrelation {

private:
    double sampling_rate;
    int data_size;

    autocorrelation() { }

    double get_acf_periodicity(double *data, int size);
    double *get_normalized_acf(double *data);

public:
    autocorrelation(double sampling_rate, int size);

    double get_pitch(double *data);

};


#endif //MPM_AUTOCORRELATION_H
