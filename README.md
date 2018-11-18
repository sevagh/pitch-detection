### Pitch detection algorithms

A collection of C++ pitch detection algorithms.

* [McLeod Pitch Method](http://miracle.otago.ac.nz/tartini/papers/A_Smarter_Way_to_Find_Pitch.pdf)
* [YIN](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf)
* Autocorrelation

Visualizations of these methods can be viewed at https://github.com/sevagh/mcleod https://github.com/sevagh/yin

YIN doesn't suffer the same weakness as the McLeod pitch method - inaccuracy at low pitches - there's no cutoff. Also, with my recent YIN-FFT implementation, the performance of the two is almost identical.

### YIN FFT approximation - 18/11/2018

The YIN paper includes the formulation for the difference function, `d_t`, for a lag tau, as expressed in terms of the autocorrelation `r_t`:

```
d_t(tau) = r_t(0) + r_(t+tau)(0) - 2*r_t(tau)
```

The term `r_(t+tau)` was confusing for me, so in my code I substituted it for

```
d_t(tau) = 2*r_t(0) - 2*r_t(tau)
```

, where `r_t(tau)` is the same FFT-based autocorrelation function I use for MPM.

This is a big performance win for YIN from **O(N^2)** to **O(NlogN)**, and all of the tests are still passing.

The accuracy didn't take a huge hit.

Time-domain YIN result:
```
$ time ./bin/stdin --sample_rate 44100 --algo yin <./sample/F-4_48000_classicalguitar.txt
Size: 174759    pitch: 342.271
closest note: F4 (349.2)

real    0m8.066s
user    0m8.020s
sys     0m0.004s
```

FFT YIN result:

```
$ time ./bin/stdin --sample_rate 44100 --algo yin <./sample/F-4_48000_classicalguitar.txt
Size: 174759    pitch: 342.238
closest note: F4 (349.2)

real    0m0.262s
user    0m0.216s
sys     0m0.043s
```

Time-domain YIN benchmark:

```
---------------------------------------------------------------
Benchmark                        Time           CPU Iterations
---------------------------------------------------------------
BM_Yin_Sinewave/1024         60976 ns      60878 ns      11130
BM_Yin_Sinewave/4096       1002633 ns    1000951 ns        688
BM_Yin_Sinewave/32768     64198002 ns   64096428 ns         11
BM_Yin_Sinewave/262144  4125974701 ns 4114026064 ns          1
BM_Yin_Sinewave/1048576 67683358752 ns 67461192598 ns          1
BM_Yin_Sinewave_BigO          0.06 N^2       0.06 N^2
BM_Yin_Sinewave_RMS              0 %          0 %
```

FFT YIN benchmark:

```
---------------------------------------------------------------
Benchmark                        Time           CPU Iterations
---------------------------------------------------------------
BM_Yin_Sinewave/1024         60657 ns      60202 ns      11223
BM_Yin_Sinewave/4096        241264 ns     239170 ns       2967
BM_Yin_Sinewave/32768      2077014 ns    2054092 ns        340
BM_Yin_Sinewave/262144    19811215 ns   19568974 ns         36
BM_Yin_Sinewave/1048576   95163598 ns   93849952 ns          7
BM_Yin_Sinewave_BigO          4.52 NlgN       4.46 NlgN
BM_Yin_Sinewave_RMS              3 %          3 %
```

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
