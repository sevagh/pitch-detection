### Pitch detection algorithms

A collection of C++ pitch detection algorithms.

* McLeod Pitch Method
* YIN
* Autocorrelation
* DFT
* Goertzel

MPM performs best on real musical instruments and voice.

### Install

Depends on [ffts](https://github.com/anthonix/ffts).

```
$ make
$ FFT_FLAG="-DPORTABLE_XCORR" make # build with no FFTS
$ sudo make install
```

Use with `#include <pitch_detection.h>` and the flag `-lpitch_detection`.

**NB**: using input vectors of size power-of-2 will perform better due FFT optimizations.

### Examples

The examples depend on [gflags](https://github.com/gflags/gflags):

```
$ sudo dnf install gflags-devel
$ make examples
```

Usage of sinewave:

```
Flags from example/sinewave.cpp:
    -algo (Algorithm to test) type: string default: "mpm"
    -frequency (Sinewave frequency in Hz) type: double default: -1
    -noise (Noise to introduce in %) type: double default: 0
    -plot (Output sinewave data to stdout) type: bool default: false
    -plot_lags (Output sinewave data with lags to stdout) type: bool
      default: false
    -quiet (Suppress most outputs) type: bool default: false
    -sample_rate (Sample rate in Hz) type: uint64 default: 48000
    -size (Sine wave size (single channel)) type: int32 default: 4096
```

Usage of stdin:

```
Flags from example/stdin.cpp:
    -algo (Algorithm to test) type: string default: "mpm"
    -plot_lags (Output sinewave data with lags to stdout) type: bool
      default: false
    -sample_rate (Sample rate in Hz) type: uint64 default: 48000
```
