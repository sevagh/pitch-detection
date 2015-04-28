#include <iostream>
#include "lib/sinewave/sinegenerator.h"

using namespace std;

int main() {
    sinegenerator sinegenerator1 = sinegenerator(48000, 1000);

    sinegenerator1.generate_tone_dual_channel();

    int i = 0;

    for (i = 0; i < sinegenerator1.size; i++) {
        std::cout << sinegenerator1.tone[i] << endl;
    }

    sinegenerator1.cleanup();

    exit(0);
}