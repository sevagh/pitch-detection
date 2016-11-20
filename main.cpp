#include <iostream>
#include <fstream>
#include "pitch_detector.h"
#include "sinegenerator.h"
#include "mpm.h"
#include "goertzel.h"
#include "dft.h"
#include "autocorrelation.h"
#include "mp3read.h"
#include "yin.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("usage: %s <testbench> <algo-name>\n", argv[0]);
		printf("\t\ttestbenches: mp3, sinewave\n");
		printf("\t\talgos: mpm, goertzel, dft, autocorrelation yin\n");
		exit(-1);
	}

	PitchDetector *pitch_detector;

	string testbench_str = std::string(argv[1]);
	string algo_str = std::string(argv[2]);

	if (algo_str == "mpm") {
		pitch_detector = new mpm();
	} else if (algo_str == "goertzel") {
		pitch_detector = new goertzel();
	} else if (algo_str == "dft") {
		pitch_detector = new dft();
	} else if (algo_str == "autocorrelation") {
		pitch_detector = new autocorrelation();
	} else if (algo_str == "yin") {
		pitch_detector = new yin();
	} else {
		std::cout << algo_str << " is not a valid algo\n";
		exit(-1);
	}

	if (testbench_str == "mp3") {
		read_mp3_file((char *) "./tests/guitar_eadgbe.mp3", pitch_detector);
	} else if (testbench_str == "sinewave") {
		std::ifstream input( "./tests/pitches.txt" );
		double sine_freq;
		while (input >> sine_freq) {
			sinegenerator sinegen = sinegenerator(48000, sine_freq);
			sinegen.generate_tone();
			pitch_detector->init(48000, sinegen.size_single_channel);
			double pitch = pitch_detector->get_pitch(sinegen.tone_single_channel);
			printf("Sinewave freq: %f\tpitch: %f\n", sine_freq, pitch);
			pitch_detector->cleanup();
			sinegen.cleanup();
		}
	} else {
		std::cout << testbench_str << " is not a valid testbench\n";
		exit(-1);
	}

	exit(0);
}
