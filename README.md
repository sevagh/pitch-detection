### Pitch detection algorithms

Autocorrelation-based C++ pitch detection algorithms with **O(nlogn)** running time:

* McLeod pitch method - [2005 paper](http://miracle.otago.ac.nz/tartini/papers/A_Smarter_Way_to_Find_Pitch.pdf) - [visualization](./misc/mcleod)
* YIN(-FFT) - [2002 paper](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf) - [visualization](./misc/yin)
* Probabilistic YIN - [2014 paper](https://www.eecs.qmul.ac.uk/~simond/pub/2014/MauchDixon-PYIN-ICASSP2014.pdf) - *partial implementation*\*

\*: The second part of the PYIN paper uses an HMM to introduce temporal tracking. I've chosen not to implement it in this codebase, because that's more in the realm of a _transcriber_, while I'm choosing to limit this project to pitch tracking for single frames of data.

### Build and install

Using this project should be as easy as `make && sudo make install` on Linux with a modern GCC - I don't officially support other platforms.

This project depends on [ffts](https://github.com/anthonix/ffts). To run the tests, you need [googletest](https://github.com/google/googletest), and run `make -C test/ && ./test/test`. To run the bench, you need [google benchmark](https://github.com/google/benchmark), and run `make -C test/ bench && ./test/bench`.

### Usage

The code is lightly documented in the [public header file](./include/pitch_detection.h). Compile your code with `-lpitch_detection`.

The namespaces are `pitch` and `pitch_alloc`. The functions and classes are templated for `<double>` and `<float>` support.

The `pitch` namespace functions are for automatic buffer allocation:

```c++
#include <pitch_detection.h>

//std::vector<double> audio_buffer with sample rate e.g. 48000

double pitch_yin = pitch::yin<double>(audio_buffer, 48000);
double pitch_mpm = pitch::mpm<double>(audio_buffer, 48000);

//pyin emits a vector of (pitch, probability) pairs
std::vector<std::pair<double, double>> pitches_pyin = pitch::pyin<double>(audio_buffer, 48000);
```

If you want to detect pitch for multiple audio buffers of a uniform size, you can do more manual memory control with the `pitch_alloc` namespace:

```c++
#include <pitch_detection.h>

//buffers have fixed length e.g. 48000, same as sample rate

pitch_alloc::Mpm<double> ma(48000);
pitch_alloc::Yin<double> ya(48000);

for (int i = 0; i < 10000; ++i) {
        //std::vector<double> audio_buffer size 48000 sample rate 48000

        auto pitch_yin = pitch_alloc::yin(audio_buffer, 48000, &ya);
        auto pitch_mpm = pitch_alloc::mpm(audio_buffer, 48000, &ma);

        auto pitch_pyin = pitch_alloc::yin(audio_buffer, 48000, &ya);
}
```

### Magic constants

I recently standardized my treatment of magic constants. They now go into the anonymous private namespaces `pyin_consts`, `yin_consts`, `mpm_consts`.

#### McLeod magic constants

```c++
// anonymous namespace mpm_consts in src/mpm.cpp

Cutoff = 0.93
Small_Cutoff = 0.5
Lower_Pitch_Cutoff = 80.0
```

Source: https://github.com/JorenSix/TarsosDSP

#### YIN magic constants

```c++
// anonymous namespace yin_consts in src/yin.cpp

Threshold = 0.20
```

Source: https://github.com/JorenSix/TarsosDSP

#### PYIN magic constants


```c++
// anonymous namespace pyin_consts in src/pyin.cpp

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
