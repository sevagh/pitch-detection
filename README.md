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
$ ./bin/sinewave
Usage: sinewave <freq> <algo> <size> <sample_rate>
$ ./bin/sinewave 1337 yin 8092 48000
Freq: 1337      pitch: 1337.48
```

Use https://github.com/sevagh/sine-generator to generate sine waves and feed the result to stdin.cpp:

```
$ wget -qO- "https://raw.githubusercontent.com/sevagh/sine-generator/master/sine.sh" | sh -s -- 4096 1337 | ./bin/stdin mpm 48000
Size: 4096
Pitch: 1337.01
```
