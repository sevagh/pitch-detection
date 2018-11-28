### Pitch detection algorithms

A collection of autocorrelation-based C++ pitch detection algorithms with **O(nlogn)** running time.

* [McLeod Pitch Method](http://miracle.otago.ac.nz/tartini/papers/A_Smarter_Way_to_Find_Pitch.pdf)
* YIN - [paper](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf) - [visualization](./misc/yin)
* Autocorrelation

### Build and install

Using this project should be as easy as `make && sudo make install` on Linux with a modern GCC - I don't officially support other platforms.

This project depends on [ffts](https://github.com/anthonix/ffts).

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
$ ./bin/stdin --sample_rate 44100 <sample/E2_44100_acousticguitar.txt
Size: 4095      pitch: 82.5838
closest note: E2 (82.41)
```

### Tests

To run the tests, you need [googletest](https://github.com/google/googletest), and run `make test && ./bin/test`:

```
[----------] 5 tests from MpmInstrumentTest
[ RUN      ] MpmInstrumentTest.Violin_A4_44100
[       OK ] MpmInstrumentTest.Violin_A4_44100 (234 ms)
[ RUN      ] MpmInstrumentTest.Piano_B4_44100
[       OK ] MpmInstrumentTest.Piano_B4_44100 (101 ms)
[ RUN      ] MpmInstrumentTest.Piano_D4_44100
[       OK ] MpmInstrumentTest.Piano_D4_44100 (116 ms)
[ RUN      ] MpmInstrumentTest.Acoustic_E2_44100
[       OK ] MpmInstrumentTest.Acoustic_E2_44100 (4 ms)
[ RUN      ] MpmInstrumentTest.Classical_FSharp4_48000
[       OK ] MpmInstrumentTest.Classical_FSharp4_48000 (239 ms)
[----------] 5 tests from MpmInstrumentTest (695 ms total)

[----------] 5 tests from YinInstrumentTest
[ RUN      ] YinInstrumentTest.Violin_A4_44100
[       OK ] YinInstrumentTest.Violin_A4_44100 (7416 ms)
[ RUN      ] YinInstrumentTest.Piano_B4_44100
[       OK ] YinInstrumentTest.Piano_B4_44100 (1153 ms)
[ RUN      ] YinInstrumentTest.Piano_D4_44100
[       OK ] YinInstrumentTest.Piano_D4_44100 (2446 ms)
[ RUN      ] YinInstrumentTest.Acoustic_E2_44100
[       OK ] YinInstrumentTest.Acoustic_E2_44100 (6 ms)
[ RUN      ] YinInstrumentTest.Classical_FSharp4_48000
[       OK ] YinInstrumentTest.Classical_FSharp4_48000 (7353 ms)
[----------] 5 tests from YinInstrumentTest (18374 ms total)
```

### Bench

To run the bench, you need [google benchmark](https://github.com/google/benchmark), and run `make bench && ./bin/bench`:

```
---------------------------------------------------------------
Benchmark                        Time           CPU Iterations
---------------------------------------------------------------
BM_Yin_Sinewave/1024         61782 ns      61237 ns      10621
BM_Yin_Sinewave/4096        241637 ns     239616 ns       2935
BM_Yin_Sinewave/32768      2054685 ns    2035207 ns        346
BM_Yin_Sinewave/262144    19950330 ns   19846980 ns         36
BM_Yin_Sinewave/1048576   94049749 ns   93521579 ns          7
BM_Yin_Sinewave_BigO          4.47 NlgN       4.45 NlgN
BM_Yin_Sinewave_RMS              2 %          2 %
BM_Mpm_Sinewave/1024         63301 ns      62817 ns      11099
BM_Mpm_Sinewave/4096        211330 ns     210247 ns       3308
BM_Mpm_Sinewave/32768      1823055 ns    1814052 ns        384
BM_Mpm_Sinewave/262144    17376578 ns   17284055 ns         40
BM_Mpm_Sinewave/1048576   97836061 ns   97294387 ns          7
BM_Mpm_Sinewave_BigO          4.62 NlgN       4.59 NlgN
BM_Mpm_Sinewave_RMS              9 %          9 %
```
