#include <iostream>
#include <cstdlib>
#include <fstream>
#include "sinegenerator.h"
#include "pitch_detector.h"

#include "testbench.h"
#ifdef FFMPEG_ENABLED
#include "mp3read.h"
#endif

void testbench(std::string testbench_type, std::string algo_str)
{
	if (testbench_type == "mp3") {
#ifdef FFMPEG_ENABLED
		read_mp3_file((char *) "./tests/guitar_eadgbe.mp3", algo_str);
#else
		std::cout << "Compiled without ffmpeg/libav, cannot use this feature\n";
		std::exit(-1);
#endif
	} else if (testbench_type == "sinewave") {
        PitchDetector *pitch_detector = get_pitch_detector(algo_str, 1024, 48000);
		std::ifstream input("./tests/pitches.txt");
		double sine_freq;
		while (input >> sine_freq) {
			sinegenerator sinegen = sinegenerator(48000, sine_freq);
			sinegen.generate_tone();
			double pitch = pitch_detector->get_pitch(sinegen.tone_single_channel);
			printf("Sinewave freq: %f\tpitch: %f\n", sine_freq, pitch);
		}
		std::exit(0);
	} else {
		std::cout << testbench_type << " is not a valid testbench\n";
		std::exit(-1);
	}
}
