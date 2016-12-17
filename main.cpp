#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <gflags/gflags.h>
#include "sinegenerator.h"

#ifdef FFMPEG_ENABLED
#include "file.h"
#endif

#define DEFAULT_PATH "/dev/null"

DEFINE_double(freq, -1, "Sinewave frequency");
DEFINE_string(algo, "mpm", "Algorithm to test");
static bool validate_path(const char* flagname, const std::string& path)
{
	std::ifstream file(path);
	return file.good();
}
DEFINE_string(path, DEFAULT_PATH, "Audio file path");
DEFINE_validator(path, &validate_path);

int main(int argc, char **argv)
{
	gflags::SetUsageMessage("help\n");
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	bool freq_valid = (FLAGS_freq > 0.0);
	bool path_valid = (FLAGS_path.compare(DEFAULT_PATH) != 0);
	if (freq_valid == path_valid) {
		std::cerr << "Please define valid --freq OR --path" << std::endl;
		return -1;
	} else if (freq_valid) {
		sinegenerator sinegen = sinegenerator(48000, FLAGS_freq);
		sinegen.validate_pitch(FLAGS_algo);
	} else if (path_valid) {
#ifdef FFMPEG_ENABLED
		read_audio_file(FLAGS_path, FLAGS_algo);
#else
		std::cerr << "Compiled without ffmpeg/libav, cannot use this feature\n";
		std::exit(-1);
#endif
	}
	return 0;
}
