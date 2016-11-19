# Pitch Detection

Collection of C++ pitch detection algorithms.

### Algorithms

* McLeod Pitch Method (time domain)
* YIN
* Goertzel
* DFT/FFT
* Autocorrelation (FFT)

YIN and McLeod inspired by [TarsosDSP](https://github.com/JorenSix/TarsosDSP).

### Build

CMake project.

    apt-get install make cmake gcc g++ libfftw3-dev libavcodec-dev libavformat-dev libavutil-dev
    cmake . && make && ./pitch_detection

### Run

	$ ./pitch_detection mpm
	[mp3 @ 0x564d21e1d200] Skipping 0 bytes of junk at 576.
	tstamp: 11154	82.264716
	tstamp: 12146	82.458491
	tstamp: 12173	82.441581
	[...]
	tstamp: 42971	123.542758
	tstamp: 43023	123.547423
	tstamp: 43102	123.521943
