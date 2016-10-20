# Pitch Detection

Collection of C++ pitch detection algorithms.

### Algorithms

* McLeod Pitch Method (time domain)
* Goertzel
* DFT/FFT
* Autocorrelation (FFT)


## Install

CMake project. Comes with [cqfd](https://github.com/savoirfairelinux/cqfd) files for easy building:

    $ cqfd init
    $ cqfd
    ...
    [100%] Linking CXX executable pitch_detection
    [100%] Built target pitch_detection 
    $ ./pitch_detection
    MPM (time-domain) pitch: 8322.471641
    Autocorrelation pitch: 8355.990220
    ...

Without cqfd/Docker:

    apt-get install libfftw3-dev && cmake . && make && ./pitch_detection
