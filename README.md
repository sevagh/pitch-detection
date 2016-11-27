# Pitch Detection

Collection of C++ pitch detection algorithms, along with some testing capabilities:

* Test against an mp3 clip of a guitar EADGBE (**ffmpeg/libav required to compile with this feature**)
* Test against generated sine waves

The algorithms are:

* McLeod Pitch Method (time domain) *
* YIN *
* Goertzel
* DFT/FFT
* Autocorrelation (FFT) (**fftw3 required to compile with this algorithm**)

\*YIN and McLeod inspired by [TarsosDSP](https://github.com/JorenSix/TarsosDSP).

MPM performs best on the guitar clip - see [Pitcha](https://github.com/sevagh/Pitcha) and [transcriber](https://github.com/sevagh/transcriber), two of my projects which use the MPM. **I strongly recommend using the MPM for any musical instrument pitch detection project**. A problem with the MPM is the low pitch cutoff.

The rest are included for posterity. Goertzel and DFT are dogshit at musical instruments. Also, my autocorrelation implementation is buggy. 

### Build

CMake project.

    apt-get install make cmake gcc g++ libfftw3-dev libavcodec-dev libavformat-dev libavutil-dev
    cmake . && make && ./pitch_detection

### Housekeeping

The script `housekeeping.sh` has some functions:

* clean: clean cmake/build files
* quality: check quality with `cppcheck` and `cppclean`
* build: run various flavored builds (with/without FFMPEG/FFTW in Docker) to test if the compilation is working
