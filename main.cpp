#include <iostream>
#include "lib/sinewave/sinegenerator.h"
#include "lib/mpm/mpm.h"

using namespace std;

int main() {
    sinegenerator sinegenerator1 = sinegenerator(48000, 2000);
    mpm mpm1 = mpm(48000, sinegenerator1.size_single_channel);

    sinegenerator1.generate_tone();
    double pitch = mpm1.get_pitch(sinegenerator1.tone_single_channel);

    std::cout << ("Pitch: %f", pitch) << endl;

    sinegenerator1.cleanup();
    mpm1.cleanup();

    exit(0);
}