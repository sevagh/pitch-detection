# Pitch Detection

Collection of C++ pitch detection algorithms.

### Algorithms

* McLeod Pitch Method (time domain)
* Goertzel
* DFT/FFT
* Autocorrelation (FFT)


### Build & run

CMake project.

    apt-get install make cmake gcc g++ libfftw3-dev libavcodec-dev libavformat-dev libavutil-dev
    cmake . && make && ./pitch_detection

Comes with [cqfd](https://github.com/savoirfairelinux/cqfd) config files for easy building:

    $ cqfd init
    $ cqfd
    ...
    [100%] Linking CXX executable pitch_detection
    [100%] Built target pitch_detection 
    $ ./pitch_detection mpm
    MPM (time-domain) pitch: 8322.471641
    $ ./pitch_detection autocorrelation
    Autocorrelation pitch: 8355.990220
    ...
