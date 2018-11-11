### Pitch detection algorithms

A collection of C++ pitch detection algorithms.

* McLeod Pitch Method
* YIN
* Autocorrelation
* DFT
* Goertzel

MPM performs best on real musical instruments and voice.

### Install

Depends on [ffts](https://github.com/anthonix/ffts). Input vectors of size power-of-2 will perform better due to FFTS optimizations.

Instructions are generated from a bare Fedora 28 Docker container.

With ffts:

```
$ dnf install git cmake make gcc gcc-c++ gflags
$ git clone https://github.com/anthonix/ffts
$ cd ffts
$ mkdir build && cd build && cmake .. && make && make install
$ cd ../../
$ git clone https://github.com/sevagh/pitch-detection
$ cd pitch-detection
$ make
$ sudo make install
$ make examples
```

Without ffts:

```
$ dnf install git make gcc gcc-c++ gflags
$ git clone https://github.com/sevagh/pitch-detection
$ cd pitch-detection 
$ FFT_FLAG="-DPORTABLE_XCORR" make # build with no FFTS
$ sudo make install
$ make examples
```

Use with `#include <pitch_detection.h>` and the flag `-lpitch_detection`.

Use the examples at `bin/`.
