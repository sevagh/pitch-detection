### Pitch detection algorithms

A collection of autocorrelation-based C++ pitch detection algorithms with **O(nlogn)** running time.

* McLeod pitch method - [paper](http://miracle.otago.ac.nz/tartini/papers/A_Smarter_Way_to_Find_Pitch.pdf) - [visualization](./misc/mcleod)
* YIN - [paper](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf) - [visualization](./misc/yin)
* Autocorrelation

### Build and install

Using this project should be as easy as `make && sudo make install` on Linux with a modern GCC - I don't officially support other platforms.

This project depends on [ffts](https://github.com/anthonix/ffts). To run the tests, you need [googletest](https://github.com/google/googletest), and run `make test && ./bin/test`. To run the bench, you need [google benchmark](https://github.com/google/benchmark), and run `make bench && ./bin/bench`. To profile with perf, you can use `make profile` to compile the benchmarks with `-fno-omit-frame-pointer`.

### Usage

Compile your code with `-lpitch_detection`:

```c++
#include <pitch_detection.h>

//std::vector<double> audio_buffer with sample rate e.g. 48000

auto pitch_yin = pitch::yin(audio_buffer, 48000);
auto pitch_mpm = pitch::mpm(audio_buffer, 48000);
```

#### Manual memory allocation

If you want to detect pitch for multiple audio buffers of a uniform size, you can allocate the pitch buffers once:

```c++
#include <pitch_detection.h>

//buffers have fixed length e.g. 48000, same as sample rate

MpmAlloc ma(48000);
YinAlloc ya(48000);

for (int i = 0; i < 10000; ++i) {
        //std::vector<double> audio_buffer size 48000 sample rate 48000

        auto pitch_yin = pitch_manual_alloc::yin(audio_buffer, 48000, &ya);
        auto pitch_mpm = pitch_manual_alloc::mpm(audio_buffer, 48000, &ma);
}
```

#### Advantages of manual memory allocation

The auto allocation strategy performs hundreds of thousands of allocations:

![auto-alloc](./misc/membench/auto-alloc.png)

Manual allocation, as expected, performs less allocations by several orders of magnitude:

![manual-alloc](./misc/membench/manual-alloc.png)

### Examples

To build the examples, you need [gflags](https://github.com/gflags/gflags). Build them with `make example`. Each example binary has full `--help` text but here's some quick usage for,

sinewave:

```
$ ./bin/sinewave --frequency 1234 --algo mpm --size 8092
FFTS performs better with power-of-two sizes
Size: 8092      freq: 1234      pitch: 1233.99
closest note: Eb6 (1245)
```

stdin:

```
$ ./bin/stdin --sample_rate 44100 <./misc/sample/E2_44100_acousticguitar.txt
Size: 4095      pitch: 82.5838
closest note: E2 (82.41)
```
