# Pitch Detection

Collection of C++ pitch detection algorithms based on autocorrelation.

The algorithms are:

* McLeod Pitch Method *
* YIN *
* Autocorrelation

\*YIN and McLeod inspired by [TarsosDSP](https://github.com/JorenSix/TarsosDSP).

MPM performs best on guitar sounds - see [Pitcha](https://github.com/sevagh/Pitcha) and [mcleod-pitch-method](https://github.com/sevagh/mcleod-pitch-method), two of my projects which use the MPM. **I strongly recommend using the MPM for any musical instrument pitch detection project**. A problem with the MPM is the low pitch cutoff.

### Examples

`make example`

Sinewave: generate a sinewave and feed it to pitch detection

```
sevagh:pitch-detection $ ./bin/sinewave --freq 1337 --algo mpm
Freq: 1337      pitch: 1337.01
```

Stdin: get the pitch of an array piped through stdin

Goes well with [github.com/sevagh/sine-generator](https://github.com/sevagh/sine-generator).

```
sevagh:pitch-detection $ ~/repos/sine-generator/sine.sh 4096 1337 | ./bin/stdin
pitch: 1337.01
```

### Build/install

Switched from CMake to Make: `make && sudo make install`

To use in your code:

```
$ head -n1 mycode.cpp
#include <pitch_detection.hpp>
...

g++ [...] -lpitch_detection
```
