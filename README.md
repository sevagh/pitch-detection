### Pitch detection algorithms

Autocorrelation-based C++ pitch detection algorithms with **O(nlogn)** running time and a C API:

* McLeod pitch method - [2005 paper](http://miracle.otago.ac.nz/tartini/papers/A_Smarter_Way_to_Find_Pitch.pdf) - [visualization](./misc/mcleod)
* YIN(-FFT) - [2002 paper](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf) - [visualization](./misc/yin)
* Probabilistic YIN - [2014 paper](https://www.eecs.qmul.ac.uk/~simond/pub/2014/MauchDixon-PYIN-ICASSP2014.pdf) - *partial implementation*\*
* Probabilistic MPM - [my own invention](https://github.com/sevagh/probabilistic-mcleod)

\*: The second part of the PYIN paper uses an HMM to introduce temporal tracking. I've chosen not to implement it in this codebase, because that's more in the realm of a _transcriber_, while I'm choosing to limit this project to pitch tracking for single frames of data.

### Build and install

Using this project should be as easy as `make && sudo make install` on Linux with a modern GCC - I don't officially support other platforms.

This project depends on [ffts](https://github.com/anthonix/ffts). To run the tests, you need [googletest](https://github.com/google/googletest), and run `make -C test/ && ./test/test`. To run the bench, you need [google benchmark](https://github.com/google/benchmark), and run `make -C test/ bench && ./test/bench`.

Build and install pitch_detection, run the tests, and build the examples:

```
# build libpitch_detection.so
make clean all

# build tests and benches
make -C test clean all

# run tests and benches 
./test/test
./test/bench
./test/benchmem

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

### Magic constants

I recently standardized my treatment of magic constants. They now go into the anonymous private namespaces `pyin_consts`, `yin_consts`, `mpm_consts`.

#### MPM

```c++
// src/mpm.cpp
Cutoff = 0.93
Small_Cutoff = 0.5
Lower_Pitch_Cutoff = 80.0
```

Source: https://github.com/JorenSix/TarsosDSP

#### YIN

```c++
// src/yin.cpp
Threshold = 0.20
```

Source: https://github.com/JorenSix/TarsosDSP

#### PYIN


```c++
// src/pyin.cpp
Pa = 0.01
N_Thresholds = 100
```

Source: [the paper, section 2.1, page 2](https://www.eecs.qmul.ac.uk/~simond/pub/2014/MauchDixon-PYIN-ICASSP2014.pdf)

```c++
Beta_Distribution[100] = {0.012614, 0.022715, 0.030646,
    0.036712, 0.041184, 0.044301, 0.046277, 0.047298, 0.047528, 0.047110,
    0.046171, 0.044817, 0.043144, 0.041231, 0.039147, 0.036950, 0.034690,
    0.032406, 0.030133, 0.027898, 0.025722, 0.023624, 0.021614, 0.019704,
    0.017900, 0.016205, 0.014621, 0.013148, 0.011785, 0.010530, 0.009377,
    0.008324, 0.007366, 0.006497, 0.005712, 0.005005, 0.004372, 0.003806,
    0.003302, 0.002855, 0.002460, 0.002112, 0.001806, 0.001539, 0.001307,
    0.001105, 0.000931, 0.000781, 0.000652, 0.000542, 0.000449, 0.000370,
    0.000303, 0.000247, 0.000201, 0.000162, 0.000130, 0.000104, 0.000082,
    0.000065, 0.000051, 0.000039, 0.000030, 0.000023, 0.000018, 0.000013,
    0.000010, 0.000007, 0.000005, 0.000004, 0.000003, 0.000002, 0.000001,
    0.000001, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000};
```

Source: [vamp plugin source code that accompanies the paper](https://code.soundsoftware.ac.uk/projects/pyin) and [Essentia](https://github.com/MTG/essentia/pull/809/files)

#### PMPM

```c++
// src/pmpm.cpp

// probability that any pitch candidate computed with k between 0.8 and 1.0, distributed into 20 increments of 0.01, is the right answer
Probability_Distribution = 0.20

// we start from 0.8 and add increments of 0.01
Cutoff_Begin = 0.8
Cutoff_Step = 0.01

// initial probability, borrowed from PYin
Pa = 0.01

// same as regular mpm
Small_Cutoff = 0.5
Lower_Pitch_Cutoff = 80.0
```

Source: https://github.com/sevagh/probabilistic-mcleod
