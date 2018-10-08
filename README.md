### Pitch detection algorithms

A collection of C++ pitch detection algorithms.

* McLeod Pitch Method
* YIN
* Autocorrelation
* DFT
* Goertzel

MPM performs best on real musical instruments and voice; see [pitcha](https://github.com/sevagh/pitcha) and [transcribe](https://github.com/sevagh/transcribe).

### Install

Depends on [ffts](https://github.com/anthonix/ffts).

```
$ make
$ FFT_FLAG="-DPORTABLE_XCORR" make # build with no FFTS
$ sudo make install
```

Use with `#include <pitch_detection.h>` and the flag `-lpitch_detection`.


To use in your code:

```
$ head -n1 main.cpp
#include <pitch_detection.h>
$ g++ main.cpp -lpitch_detection
```

**NB**: using input vectors of size power-of-2 will perform better due FFT optimizations.

### Sinewave example

Depends on [gflags](https://github.com/gflags/gflags):

```
$ sudo dnf install gflags-devel
$ make examples
```

Usage:

```
$ ./bin/sinewave --help
Flags from example/sinewave.cpp:
    -algo (Algorithm to test) type: string default: "mpm"
    -frequency (Sinewave frequency in Hz) type: double default: -1
    -noise (Noise to introduce in %) type: double default: 0
    -sample_rate (Sample rate in Hz) type: uint64 default: 48000
    -size (Sine wave size (single channel)) type: uint64 default: 4096
```
