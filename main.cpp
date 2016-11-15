#include <iostream>
#include "src/sinegenerator.h"
#include "src/mpm.h"
#include "src/goertzel.h"
#include "src/dft.h"
#include "src/autocorrelation.h"
#include "src/pitch_detector.h"
#include "src/mp3read.h"

using namespace std;

int main(int argc, char **argv)
{
	double pitch = 0.0f;
	double frequency = 8351;

	if (argc < 2) {
		printf("usage: %s\t<algo-name>\n", argv[0]);
		printf("\talgos: mpm, goertzel, dft, autocorrelation\n");
		exit(-1);
	}

	PitchDetector *pitch_detector;

	if (std::string(argv[1]) == "mpm") {
		pitch_detector = new mpm(48000, 2048);
	} else if (std::string(argv[1]) == "goertzel") {
		pitch_detector = new goertzel(48000, 2048);
	} else if (std::string(argv[1]) == "dft") {
		pitch_detector = new dft(48000, 2048);
	} else if (std::string(argv[1]) == "autocorrelation") {
		pitch_detector = new autocorrelation(48000, 2048);
	} else {
		printf("%s is not a valid algo\n", argv[1]);
		exit(0);
	}

	/*
	pitch = pitch_detector
		->get_pitch(sinegenerator1.tone_single_channel);
	printf("%s pitch: %f\n", argv[1], pitch);
	sinegenerator sinegenerator1 = sinegenerator(48000,
				       frequency);
	sinegenerator1.generate_tone();
	sinegenerator1.cleanup();
	*/

	read_mp3_file((char *) "./tests/guitar_eadgbe.mp3", pitch_detector);

	pitch_detector->cleanup();

	exit(0);
}
