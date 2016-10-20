//
// Created by sevag on 4/27/15.
//

#ifndef MPM_SINEGENERATOR_H
#define MPM_SINEGENERATOR_H

class sinegenerator {

private:
    double sampling_rate;
    double inv_sampling_rate;
    double frequency;

    sinegenerator() { }

public:
    int size_single_channel;
    int size_dual_channel;

    double *tone_dual_channel;
    double *tone_single_channel;

    sinegenerator(double sampling_rate, double frequency);

    void generate_tone();
    void cleanup();

};

#endif //MPM_SINEGENERATOR_H
