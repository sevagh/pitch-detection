# Pitch Detection

Collection of C++ pitch detection algorithms based on autocorrelation, along with some testing capabilities:

* Test against an audio clip (**ffmpeg/libav required to compile with this feature**)
* Test against generated sine waves

The algorithms are:

* McLeod Pitch Method (time domain) *
* YIN *
* Autocorrelation

\*YIN and McLeod inspired by [TarsosDSP](https://github.com/JorenSix/TarsosDSP).

MPM performs best on guitar sounds - see [Pitcha](https://github.com/sevagh/Pitcha) and [media-util](https://github.com/sevagh/media-util), two of my projects which use the MPM. **I strongly recommend using the MPM for any musical instrument pitch detection project**. A problem with the MPM is the low pitch cutoff.

### Dependency - libxcorr

I refactored out the autocorrelation implementation to leverage [my fork of libxcorr](https://github.com/sevagh/libxcorr).

### Deprecating Goertzel/DFT

In previous commits you can view Goertzel and DFT for posterity, but these are not pitch detection algorithms per se so I removed them.

These algorithms compute one frequency at a time, so I was using them in an inefficient way (looping through `0 <= possible_pitch <= 4200` and finding out which had the most energy).

Autocorrelation-based algorithms are the real deal.

### Build

CMake project.

    apt-get install make cmake gcc g++ libfftw3-dev libavcodec-dev libavformat-dev libavutil-dev libgflags-dev
    cmake . && make && ./pitch_detection
