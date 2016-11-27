#include <iostream>
#include "pitch_detector.h"
#include "testbench.h"

#define VERSION "0.0.0"

static int print_version(std::string version)
{
	std::string version_string;
	version_string.append(version);
#ifdef FFTW_ENABLED
	version_string.append("-fftw");
#endif
#ifdef FFMPEG_ENABLED
	version_string.append("-ffmpeg");
#endif
	version_string.append("\n");
	std::cout << "VERSION: " << version_string;
}

int main(int argc, char **argv)
{
	print_version(VERSION);
	if (argc < 3) {
		printf("usage: %s <testbench> <algo-name>\n", argv[0]);
		printf("\t\ttestbenches: mp3, sinewave\n");
		printf("\t\talgos: mpm, goertzel, dft, autocorrelation yin\n");
		exit(-1);
	}

	std::string testbench_str = std::string(argv[1]);
	std::string algo_str = std::string(argv[2]);

	PitchDetector *pitch_detector = get_pitch_detector(algo_str);

	testbench(testbench_str, pitch_detector);
	exit(0);
}
