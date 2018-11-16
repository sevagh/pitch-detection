### Pitch detection algorithms

A collection of C++ pitch detection algorithms.

* McLeod Pitch Method
* YIN
* Autocorrelation

MPM performs best on real musical instruments and voice.

Visualization of McLeod pitch method (and the advantages over autocorrelation) here: https://github.com/sevagh/mcleod

### Real guitar results

The file [guitar_e.txt](./guitar_e.txt) contains 4095 audio samples from a 44100Hz recording of an open low E guitar string - the data was extracted using [pydub](https://github.com/jiaaro/pydub) from a live recording. The program is the stdin example - `cat guitar_e.txt | ./bin/stdin --sample_rate 44100` 

Expected value: **82.41Hz**

Results:

| algo | result |
| ---- | ------ |
| MPM  ffts | 82.5838 |
| MPM no ffts | 82.539
| YIN | 82.6221 | 
| Autocorrelation ffts | 282.344 |
| Autocorrelation no ffts | 249.766 |

### Install and build example

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
$ make example
```

Without ffts:

```
$ dnf install git make gcc gcc-c++ gflags
$ git clone https://github.com/sevagh/pitch-detection
$ cd pitch-detection 
$ FFT_FLAG="-DPORTABLE_XCORR" make # build with no FFTS
$ sudo make install
$ make example
```

Use with `#include <pitch_detection.h>` and the flag `-lpitch_detection`.
