#include "example.h"
#include <fstream>
#include <functional>
#include <gflags/gflags.h>
#include <iostream>
#include <pitch_detection.h>
#include <util.h>
#include <utility>
#include <vector>

DEFINE_double(frequency, -1.0, "Sinewave frequency in Hz");
DEFINE_validator(frequency,
    [](const char *flagname, double value) { return (value >= 0.0); });

DEFINE_uint64(sample_rate, 48000, "Sample rate in Hz");
DEFINE_validator(sample_rate,
    [](const char *flagname, uint64_t value) { return (value >= 0); });

DEFINE_string(algo, "mpm", "Algorithm to test");
DEFINE_validator(algo, [](const char *flagname, const std::string &value) {
	return (pitch_types.find(value) != pitch_types.end());
});

DEFINE_int32(size, 4096, "Sine wave size (single channel)");
DEFINE_validator(
    size, [](const char *flagname, int value) { return (value >= 0); });

int
main(int argc, char **argv)
{
	gflags::SetUsageMessage("help\n");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	auto data =
	    util::sinewave(2 * FLAGS_size, FLAGS_frequency, FLAGS_sample_rate);

	if ((FLAGS_algo == "mpm") &&
	    !(FLAGS_size && !(FLAGS_size & (FLAGS_size - 1))))
		std::cerr << "FFTS performs better with power-of-two sizes"
		          << std::endl;

	double pitch =
	    pitch_algorithms[pitch_types[FLAGS_algo]](data, FLAGS_sample_rate);

	std::cerr << "Size: " << FLAGS_size << "\tfreq: " << FLAGS_frequency
	          << "\tpitch: " << pitch << std::endl;

	auto closest = util::closest_note(pitch);

	std::cerr << "closest note: " << std::get<0>(closest) << " ("
	          << std::get<1>(closest) << ")" << std::endl;
	return 0;
}
