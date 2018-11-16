### Pitch detection algorithms

A collection of C++ pitch detection algorithms.

* [McLeod Pitch Method](http://miracle.otago.ac.nz/tartini/papers/A_Smarter_Way_to_Find_Pitch.pdf)
* [YIN](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf)
* Autocorrelation

Visualization of McLeod pitch method (and the advantages over autocorrelation) here: https://github.com/sevagh/mcleod

### API breaking change - 15/11/2018

**N.B.** `get_pitch_yin`, `get_pitch_mpm`, `get_pitch_autocorrelation` have become `pitch::yin`, `pitch::mpm`, `pitch::autocorrelation`.

### Build and install

Using this project should be as easy as `make && sudo make install` on Linux with a modern GCC - I don't officially support other platforms.

This project depends on [ffts](https://github.com/anthonix/ffts). You can build without ffts by specifying `FFT_FLAG="-DPORTABLE_XCORR"`, which substitutes FFT autocorrelation for time-domain autocorrelation. This will severely impact the performance of MPM.

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
--------------------------------------------------------------------
Benchmark                             Time           CPU Iterations
--------------------------------------------------------------------
BM_Yin_Sinewave_128k_samples  984091850 ns  982514163 ns          1
BM_Mpm_Sinewave_128k_samples   37793353 ns   37492225 ns         19
```
