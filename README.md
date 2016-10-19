# Pitch Detection

Collection of C++ pitch detection algorithms.

### Algorithms

* McLeod Pitch Method (time domain)
* Goertzel
* DFT/FFT
* Autocorrelation (FFT)


## Install

CMake project. Comes with a Dockerfile for the FFTW3 dependencies:

    $ docker build -t "pitchcontainer" ./
    $ docker run -v "$PWD/:/pitch_detection/" -i -t pitchcontainer
    ...
    [100%] Linking CXX executable pitch_detection
    [100%] Built target pitch_detection 
    MPM (time-domain) pitch: 8322.471641
    Autocorrelation pitch: 8355.990220
    ...
