#include <iostream>
#include <cstdlib>
#include "testbench.h"

#define VERSION "0.0.0"

static void print_version(std::string version)
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
	if (argc < 3) {
		if ((argc == 2) and (std::string(argv[1]).compare(std::string("version")) == 0)) {
			print_version(VERSION);
			std::exit(0);
		}
		else {
			std::cout << "usage: " << argv[0] << " <algo-name> <testbench>\n";
			std::cout << "\t\talgos: mpm, goertzel, dft, autocorrelation yin\n";
			std::cout << "\t\ttestbenches: mp3, sinewave\n";
			std::exit(-1);
		}
	}

	std::string algo_str = std::string(argv[1]);
	std::string testbench_str = std::string(argv[2]);

	testbench(testbench_str, algo_str);
	std::exit(0);
}
