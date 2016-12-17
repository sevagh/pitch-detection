# Pitch Detection

Collection of C++ pitch detection algorithms, along with some testing capabilities:

* Test against an audio clip (**ffmpeg/libav required to compile with this feature**)
* Test against generated sine waves

The algorithms are:

* McLeod Pitch Method (time domain) *
* YIN *
* Goertzel
* DFT/FFT
* Autocorrelation (FFT) (**fftw3 required to compile with this algorithm**)

\*YIN and McLeod inspired by [TarsosDSP](https://github.com/JorenSix/TarsosDSP).

MPM performs best on guitar sounds - see [Pitcha](https://github.com/sevagh/Pitcha) and [media-util](https://github.com/sevagh/media-util), two of my projects which use the MPM. **I strongly recommend using the MPM for any musical instrument pitch detection project**. A problem with the MPM is the low pitch cutoff.

The rest are included for posterity. Goertzel and DFT are dogshit at musical instruments.

### Build

CMake project.

    apt-get install make cmake gcc g++ libfftw3-dev libavcodec-dev libavformat-dev libavutil-dev
    cmake . && make && ./pitch_detection
