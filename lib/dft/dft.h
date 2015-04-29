//
// Created by sevag on 4/28/15.
//

#ifndef MPM_DFT_H
#define MPM_DFT_H

class dft {

private:
    double sampling_rate;
    int data_size;

    dft() { }

    double dft_energy(int frequency, double *arr, int N);
    float get_snr(int frequency, double *arr, int N);

public:
    dft(double sampling_rate, int size);

    double get_pitch(double *data);

};

#endif //MPM_DFTL_H
