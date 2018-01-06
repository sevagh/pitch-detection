### PITCH DETECTION

A collection of C++ pitch detection algorithms, all based on autocorrelation with FFTW3.

* McLeod Pitch Method
* YIN
* Autocorrelation

YIN and McLeod implementations are inspired by https://github.com/JorenSix/TarsosDSP.

MPM performs best on real musical instruments and voice; see https://github.com/sevagh/Pitcha and https://github.com/sevagh/transcribe, two of my projects which use the MPM.

### EXAMPLES

`make sinewave stdin`

* Sinewave: generate a sinewave and feed it to pitch detection
* Stdin: get the pitch of an array piped through stdin

### INSTALLING

Dependent on https://github.com/sevagh/libxcorr and gflags.

`make && sudo make install`.

To use in your code:

`#include <pitch_detection.h>` and `g++ [...] -lpitch_detection`
