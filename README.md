### Pitch detection algorithms

Autocorrelation-based C++ pitch detection algorithms with **O(nlogn)** running time:

* McLeod pitch method - [2005 paper](http://miracle.otago.ac.nz/tartini/papers/A_Smarter_Way_to_Find_Pitch.pdf) - [visualization](./misc/mcleod)
* YIN - [2002 paper](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf) - [visualization](./misc/yin)
* Probabilistic YIN - [2014 paper](https://www.eecs.qmul.ac.uk/~simond/pub/2014/MauchDixon-PYIN-ICASSP2014.pdf)

### 2019 updates and goals

The McLeod pitch method has been in this project since the beginning, April 2015. YIN was first added in November 2016, followed by YIN-FFT in November 2018 and Probabilistic YIN in March 2019.

In 2019, I have additional targets:

* Write a good testbench to compare the algorithms head-to-head, based on the [Audio Degradation Toolbox](https://code.soundsoftware.ac.uk/projects/audio-degradation-toolbox)
* Add [CREPE](https://github.com/marl/crepe)

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
}
```

### Magic constants

I recently changed my treatment of magic constants (that I've copied from various places - the original paper, reference implementations, other implementations). They now go into the anonymous private namespaces `pyin_consts`, `yin_consts`, `mpm_consts`, e.g.:

```c++
namespace
{
namespace yin_consts
{
template <typename T> static const T Threshold = static_cast<T>(0.20);
}
} // namespace
```
