HMMs for temporal pitch tracking -

* https://www.eecs.qmul.ac.uk/~simond/pub/2014/MauchDixon-PYIN-ICASSP2014.pdf 
* http://ismir2003.ismir.net/papers/Orio.pdf

This tool performs temporal pitch tracking using [probabilistic pitch detection methods](https://github.com/sevagh/pitch_detection).

The heuristics of pitch bin transitions, etc. are mostly borrowed from the pYIN codebase, with modifications.

One modification is that I only consider 108 possible pitch bins - [C0 to B8](https://pages.mtu.edu/~suits/notefreqs.html), computed using [these formulae](https://pages.mtu.edu/~suits/NoteFreqCalcs.html).

I also use the [mlpack HMM library](https://mlpack.org/doc/mlpack-git/doxygen/classmlpack_1_1hmm_1_1HMM.html#a3b7ef0aafafd5d5a4300c07b110807a4) rather than copying the one from pYIN. This is to prove (to myself mostly) that a typical HMM library can be used for pitch tracking, rather than copy-pasting the pYIN custom HMM.

[Further reading](http://mac.citi.sinica.edu.tw/~yang/teaching/lecture07_pitch.pdf) indicates the importance of training data (which I do not employ).
