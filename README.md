### Pitch detection algorithms

A collection of C++ pitch detection algorithms.

* McLeod Pitch Method
* YIN
* ~~Autocorrelation~~ removed because it gives incorrect results - MPM is an enhancement on standard autocorrelation
* DFT
* Goertzel

YIN and McLeod implementations are inspired by https://github.com/JorenSix/TarsosDSP.

MPM performs best on real musical instruments and voice; see https://github.com/sevagh/Pitcha and https://github.com/sevagh/transcribe, two of my projects which use the MPM.

### Install

Dependent on FFTW3.

`make && sudo make install`.

To use in your code:

`#include <pitch_detection.h>` and `g++ [...] -lpitch_detection`

### Usage

```
Flags from example/stdin.cpp:
    -algo (Algorithm to test) type: string default: "mpm"
    -sample_rate (Input sample rate) type: double default: 48000
```

### Examples

`make examples`

Use https://github.com/sevagh/sine-generator to generate sine waves and feed the result to stdin.cpp:

```
$ wget -qO- "https://raw.githubusercontent.com/sevagh/sine-generator/master/sine.sh" | sh -s -- 4096 1337 | ./bin/stdin --sample_rate 48000
Pitch 1337.05
```

Use `examples/sinewave.cpp` for the same thing:

```
$ ./bin/sinewave --freq 1337 --algo goertzel
Freq: 1337      pitch: 1338.89
```

Alternatively some samples are available at [samples](./samples):

```
$ ./bin/stdin --algo yin <samples/1337_hz.txt
Pitch: 1337.49
```
