#include "pitch_detection/pitch_detection.h"
#include "util.h"
#include <gflags/gflags.h>
#include <iostream>

DEFINE_string(alloc, "auto", "Alloc style");
DEFINE_int32(reps, 1000, "Repetitions");

void
mpm_auto(int reps)
{
	// a full second's worth of 48000 sample rate audio
	auto data = test_util::sinewave(48000, 1337, 48000);

	for (int i = 0; i < reps; ++i) {
		pitch::mpm<double>(data, 48000);
	}
}

void
mpm_manual(int reps)
{
	// a full second's worth of 48000 sample rate audio
	auto data = test_util::sinewave(48000, 1337, 48000);

	// alloc once
	pitch_alloc::Mpm<double> ma(48000);

	for (int i = 0; i < reps; ++i) {
		ma.pitch(data, 48000);
	}
}

int
main(int argc, char **argv)
{
	gflags::SetUsageMessage("help\n");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	std::cout << "Benchmark for alloc strategy '" << FLAGS_alloc << "', "
	          << FLAGS_reps << " repetitions..." << std::endl;

	if (FLAGS_alloc == "auto") {
		mpm_auto(FLAGS_reps);
	} else if (FLAGS_alloc == "manual") {
		mpm_manual(FLAGS_reps);
	} else {
		std::cerr << "Select one of 'auto' or 'manual' for alloc" << std::endl;
		exit(1);
	}

	std::cout << "Benchmark for alloc strategy '" << FLAGS_alloc << "' complete"
	          << std::endl;
}
