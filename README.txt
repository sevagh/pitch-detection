### PITCH DETECTION

A collection of C++ pitch detection algorithms, all based on autocorrelation with FFTW3.

* McLeod Pitch Method
* YIN
* Autocorrelation

YIN and McLeod implementations are inspired by https://github.com/JorenSix/TarsosDSP.

MPM performs best on real musical instruments and voice; see https://github.com/sevagh/Pitcha and https://github.com/sevagh/transcribe, two of my projects which use the MPM.

### EXAMPLES

Use https://github.com/sevagh/sine-generator to generate sine waves and feed the result to stdin.cpp:

`make stdin && wget -qO- "https://raw.githubusercontent.com/sevagh/sine-generator/master/sine.sh" | sh -s -- 4096 1337 | ./bin/stdin --sample_rate 48000`

### INSTALLING

Dependent on https://github.com/sevagh/libxcorr and gflags.

`make && sudo make install`.

To use in your code:

`#include <pitch_detection.h>` and `g++ [...] -lpitch_detection`
