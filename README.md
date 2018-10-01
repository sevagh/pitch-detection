### Pitch detection algorithms

A collection of C++ pitch detection algorithms.

* McLeod Pitch Method
* YIN
* Autocorrelation
* DFT
* Goertzel

YIN and McLeod implementations are inspired by https://github.com/JorenSix/TarsosDSP.

MPM performs best on real musical instruments and voice; see https://github.com/sevagh/Pitcha and https://github.com/sevagh/transcribe, two of my projects which use the MPM.

### Install

**Optionally** uses https://github.com/anthonix/ffts.

`make && sudo make install`.

To build portably, without linking to ffts (i.e. using time-domain autocorrelation, check the [autocorrelation](./src/autocorrelation.cpp) source for details):

`FFT_FLAG="-DPORTABLE_XCORR" make`

To use in your code:

`#include <pitch_detection.h>` and `g++ [...] -lpitch_detection`

### Examples usage

**NB**: using input vectors of size power-of-2 will perform better due FFT optimizations.

`make examples`


sinewave:

```
$ ./bin/sinewave --help
Flags from example/sinewave.cpp:
    -algo (Algorithm to test) type: string default: "mpm"
    -frequency (Sinewave frequency in Hz) type: double default: -1
    -noise (Noise to introduce in %) type: double default: 0
    -sample_rate (Sample rate in Hz) type: uint64 default: 48000
    -size (Sine wave size (single channel)) type: uint64 default: 4096
```
