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
