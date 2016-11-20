# Pitch Detection

Collection of C++ pitch detection algorithms, along with some testing capabilities:

* Test against an mp3 clip of a guitar EADGBE
* Test against generated sine waves

The algorithms are:

* McLeod Pitch Method (time domain) *
* YIN *
* Goertzel
* DFT/FFT
* Autocorrelation (FFT)

\*YIN and McLeod inspired by [TarsosDSP](https://github.com/JorenSix/TarsosDSP).

MPM performs best on the guitar clip - see [Pitcha](https://github.com/sevagh/Pitcha) and [transcriber](https://github.com/sevagh/transcriber), two of my projects which use the MPM. **I strongly recommend using the MPM for any musical instrument pitch detection project**. A problem with the MPM is the low pitch cutoff.

The rest are included for posterity. Goertzel and DFT are dogshit at musical instruments. Also, my autocorrelation implementation is buggy. 

### Build

CMake project.

    apt-get install make cmake gcc g++ libfftw3-dev libavcodec-dev libavformat-dev libavutil-dev
    cmake . && make && ./pitch_detection
