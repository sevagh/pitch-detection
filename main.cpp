#include <iostream>
#include "pitch_detector.h"
#include "sinegenerator.h"
#include "mpm.h"
#include "goertzel.h"
#include "dft.h"
#include "autocorrelation.h"
#include "mp3read.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("usage: %s\t<algo-name>\n", argv[0]);
		printf("\talgos: mpm, goertzel, dft, autocorrelation\n");
		exit(-1);
	}

	PitchDetector *pitch_detector;

	if (std::string(argv[1]) == "mpm") {
		pitch_detector = new mpm();
	} else if (std::string(argv[1]) == "goertzel") {
		pitch_detector = new goertzel();
	} else if (std::string(argv[1]) == "dft") {
		pitch_detector = new dft();
	} else if (std::string(argv[1]) == "autocorrelation") {
		pitch_detector = new autocorrelation();
	} else {
		printf("%s is not a valid algo\n", argv[1]);
		exit(0);
	}

	read_mp3_file((char *) "./tests/guitar_eadgbe.mp3", pitch_detector);

	exit(0);
}
