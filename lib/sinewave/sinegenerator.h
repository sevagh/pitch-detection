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
    int size;
    int *tone;

    sinegenerator(double sampling_rate, double frequency);

    void generate_tone_dual_channel();
    void cleanup();
};


#endif //MPM_SINEGENERATOR_H
