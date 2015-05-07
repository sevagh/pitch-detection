#include <iostream>
#include "lib/sinewave/sinegenerator.h"
#include "lib/mpm/mpm.h"
#include "lib/goertzel/goertzel.h"
#include "lib/dft/dft.h"
#include "lib/autocorrelation/autocorrelation.h"

using namespace std;

int main() {
    double pitch = 0.0f;
    double frequency = 8351;

    sinegenerator sinegenerator1 = sinegenerator(48000, frequency);

    mpm mpm1 = mpm(48000, sinegenerator1.size_single_channel);
    goertzel goertzel1 = goertzel(48000, sinegenerator1.size_single_channel);
    dft dft1 = dft(48000, sinegenerator1.size_single_channel);
    autocorrelation autocorrelation1 = autocorrelation(48000, sinegenerator1.size_single_channel);

    sinegenerator1.generate_tone();

    //MPM time domain result
    pitch = mpm1.get_pitch(sinegenerator1.tone_single_channel);
    printf("MPM (time-domain) pitch: %f\n", pitch);

    //autocorrelation result
    pitch = autocorrelation1.get_pitch(sinegenerator1.tone_single_channel);
    printf("Autocorrelation pitch: %f\n", pitch);

    //Goertzel result
    pitch = goertzel1.get_pitch(sinegenerator1.tone_single_channel);
    printf("Goertzel pitch: %f\n", pitch);

    //DFT result
    pitch = dft1.get_pitch(sinegenerator1.tone_single_channel);
    printf("DFT pitch: %f\n", pitch);

    sinegenerator1.cleanup();
    mpm1.cleanup();

    exit(0);
}
