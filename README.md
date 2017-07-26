# Pitch Detection

Collection of C++ pitch detection algorithms based on autocorrelation, along with a sample sinewave generator.

The algorithms are:

* McLeod Pitch Method *
* YIN *
* Autocorrelation

\*YIN and McLeod inspired by [TarsosDSP](https://github.com/JorenSix/TarsosDSP).

MPM performs best on guitar sounds - see [Pitcha](https://github.com/sevagh/Pitcha) and [mcleod-pitch-method](https://github.com/sevagh/mcleod-pitch-method), two of my projects which use the MPM. **I strongly recommend using the MPM for any musical instrument pitch detection project**. A problem with the MPM is the low pitch cutoff.

### Deprecating Goertzel/DFT

In previous commits you can view Goertzel and DFT for posterity, but these are not pitch detection algorithms per se so I removed them.

These algorithms compute one frequency at a time, so I was using them in an inefficient way (looping through `0 <= possible_pitch <= 4200` and finding out which had the most energy).

Autocorrelation-based algorithms are the real deal.

### Build/install

Switched from CMake to Make: `make && sudo make install`

To use in your code:

```
$ head -n1 mycode.cpp
#include <pitch_detection.hpp>
...

g++ [...] -lpitch_detection
```
