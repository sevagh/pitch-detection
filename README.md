### Pitch detection algorithms

Autocorrelation-based C++ pitch detection algorithms with **O(nlogn) or lower** running time and a C API:

* McLeod pitch method - [2005 paper](http://miracle.otago.ac.nz/tartini/papers/A_Smarter_Way_to_Find_Pitch.pdf) - [visualization](./misc/mcleod)
* YIN(-FFT) - [2002 paper](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf) - [visualization](./misc/yin)
* Probabilistic YIN - [2014 paper](https://www.eecs.qmul.ac.uk/~simond/pub/2014/MauchDixon-PYIN-ICASSP2014.pdf) - *partial implementation*\*
* Probabilistic MPM - [my own invention](https://github.com/sevagh/probabilistic-mcleod)
* SWIPE' - [2007 paper](https://pdfs.semanticscholar.org/0fd2/6e267cfa9b6d519967ea00db4ffeac272777.pdf) - [transliterated to C++ from kylebgorman's C implementation](https://github.com/kylebgorman/swipe)\*\*

\*: The second part of the PYIN paper uses an HMM to introduce temporal tracking. I've chosen not to implement it in this codebase, because that's more in the realm of a _transcriber_, while this project is for pitch tracking for individual arrays of audio data. I wrote a [prototype](https://github.com/sevagh/hmm-pitch-smoothing).

\*\*: SWIPE' appears to be O(n) but with an enormous constant factor. The implementation complexity is much higher than MPM and YIN and it brings in additional dependencies (BLAS + LAPACK).

Suggested usage of this library can be seen in the utility [wav_analyzer](./wav_analyzer), which divides a wav file into chunks of 0.01s and checks the pitch of each chunk.

### Degraded audio tests

Output of the results using the [audio-degradation-toolbox](https://github.com/sevagh/audio-degradation-toolbox), and [wav_analyzer](./wav_analyzer) which uses [libnyquist](https://github.com/ddiakopoulos/libnyquist) for WAV decoding.

All testing files are [here](./degraded_audio_tests). The original clip is a Viola playing E3 from the [University of Iowa MIS](http://theremin.music.uiowa.edu/MIS.html). The 5 levels of degradation are as follows:

- 0: clean
- 1: pink noise with an overall SNR of 20dB
- 2: 1 + 5 passes of harmonic (quadratic) distortion
- 3: 2 + mixed a clip of restaurant background noise
- 4: 3 + 3 consecutive mp3 transcodings at 32kbps
- 5: 4 + dynamic range compression + clipping

Results (correct pitch of an E3 is 164.81):

| Algorithm  | Pitch@0 | Pitch@1 | Pitch@2 | Pitch@3 | Pitch@4 | Pitch@5
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| MPM  | 163.101  | 163.103 | -1 | -1 | -1 | -1
| YIN  | 163.102  | 163.106 | 163.088 | 163.086 | 163.083 | 163.094
| SWIPE' | 163.358 | 163.063 | 163.211 | 163.358 | 163.211 | 162.917

### Build and install

Using this project should be as easy as `make && sudo make install` on Linux with a modern GCC - I don't officially support other platforms.

This project depends on [ffts](https://github.com/anthonix/ffts) and BLAS/LAPACK. To run the tests, you need [googletest](https://github.com/google/googletest), and run `make -C test/ && ./test/test`. To run the bench, you need [google benchmark](https://github.com/google/benchmark), and run `make -C test/ bench && ./test/bench`.

Build and install pitch_detection, run the tests, and build the examples:

```
# build libpitch_detection.so
make clean all

# build tests and benches
make -C test clean all

# run tests and benches 
./test/test
./test/bench

# install the library and headers to `/usr/local/lib` and `/usr/local/include`
sudo make install

# build and run C++ example
make -C examples/cpp clean all
./examples/cpp/example

# build and run C example
make -C examples/c clean all
./examples/c/example
```

### Usage

#### C++

Read the [C++ header](./include/pitch_detection/pitch_detection.h) and [C++ example](./examples/cpp).

The namespaces are `pitch` and `pitch_alloc`. The functions and classes are templated for `<double>` and `<float>` support.

The `pitch` namespace functions perform automatic buffer allocation, while `pitch_alloc::{Yin, Mpm}` give you a reusable object (useful for computing pitch for multiple uniformly-sized buffers):

```c++
#include <pitch_detection.h>

std::vector<double> audio_buffer(8092);

double pitch_yin = pitch::yin<double>(audio_buffer, 48000);
double pitch_mpm = pitch::mpm<double>(audio_buffer, 48000);

std::vector<std::pair<double, double>> pitches_pyin = pitch::pyin<double>(audio_buffer, 48000);
std::vector<std::pair<double, double>> pitches_pmpm = pitch::pmpm<double>(audio_buffer, 48000);

pitch_alloc::Mpm<double> ma(8092);
pitch_alloc::Yin<double> ya(8092);

for (int i = 0; i < 10000; ++i) {
        auto pitch_yin = ya.pitch(audio_buffer, 48000);
        auto pitch_mpm = ma.pitch(audio_buffer, 48000);

        auto pitches_pyin = ya.probabilistic_pitches(audio_buffer, 48000);
        auto pitches_pmpm = ma.probabilistic_pitches(audio_buffer, 48000);
}
```

#### C

Read the [C header](./include/pitch_detection/cpitch_detection.h) and [C example](./examples/c).

In C, the `pitch` and `pitch_alloc` namespaces becomes `pitch_` and `pitch_alloc_` prefixes. `double` and `float` templates are now explicit structs and functions with `d/D` and `f/F` in the name.

Also, to represent `std::vector<std::pair<T, T>>` pitch candidates, there are some custom structs:

```c
struct pitch_candidates_d_t {
        long n_candidates;
        struct pitch_probability_pair_d_t *candidates;
};

struct pitch_probability_pair_d_t {
        double pitch;
        double probability;
};
```

Here are the above C++ examples, transliterated using the C API:

```c
#include <cpitch_detection.h>

double audio_buffer[8092];

double pitch_yin = pitch_yin_d(audio_buffer, 8092, 48000);
double pitch_mpm = pitch_mpm_d(audio_buffer, 8092, 48000);

//pyin and pmpm emit struct
struct pitch_candidates_d_t * pitches_pmpm = pitch_pmpm_d(audio_buffer, 8092, 48000);
struct pitch_candidates_d_t * pitches_pyin = pitch_pyin_d(audio_buffer, 8092, 48000);

struct Mpm_d_t ma = NewMpmD(8092);
struct Yin_d_t ya = NewYinD(8092);

for (int i = 0; i < 10000; ++i) {
        double pitch_yin = ya.pitch(audio_buffer, 48000);
        double pitch_mpm = ma.pitch(audio_buffer, 48000);

        struct pitch_candidates_d_t * pitches_pmpm = pitch_alloc_pmpm_d(ma, audio_buffer, 48000);
        struct pitch_candidates_d_t * pitches_pyin = pitch_alloc_pyin_d(ya, audio_buffer, 48000);
}
```
