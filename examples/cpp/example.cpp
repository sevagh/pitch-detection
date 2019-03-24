#include "pitch_detection/pitch_detection.h"
#include <iostream>

int
main(int argc, char **argv)
{
	// double + function
	std::vector<double> test_data_d(8092, 5000.0);
	auto pitch_mpm_d = pitch::mpm(test_data_d, 48000);
	auto pitch_yin_d = pitch::yin(test_data_d, 48000);

	auto pitches_pmpm_d = pitch::pmpm(test_data_d, 48000);
	auto pitches_pyin_d = pitch::pyin(test_data_d, 48000);

	std::cout << "mpm pitch: " << pitch_mpm_d << std::endl;
	std::cout << "yin pitch: " << pitch_yin_d << std::endl;

	for (auto pmpm_candidate : pitches_pmpm_d)
		std::cout << "pmpm pitch candidate: " << pmpm_candidate.first << ","
		          << pmpm_candidate.second << std::endl;

	for (auto pyin_candidate : pitches_pyin_d)
		std::cout << "pyin pitch candidate: " << pyin_candidate.first << ","
		          << pyin_candidate.second << std::endl;

	// double + classes
	pitch_alloc::Mpm<double> ma_d(8092);
	pitch_alloc::Yin<double> ya_d(8092);

	pitch_mpm_d = ma_d.pitch(test_data_d, 48000);
	pitch_yin_d = ya_d.pitch(test_data_d, 48000);

	pitches_pmpm_d = ma_d.probabilistic_pitches(test_data_d, 48000);
	pitches_pyin_d = ya_d.probabilistic_pitches(test_data_d, 48000);

	std::cout << "mpm pitch: " << pitch_mpm_d << std::endl;
	std::cout << "yin pitch: " << pitch_yin_d << std::endl;

	for (auto pmpm_candidate : pitches_pmpm_d)
		std::cout << "pmpm pitch candidate: " << pmpm_candidate.first << ","
		          << pmpm_candidate.second << std::endl;

	for (auto pyin_candidate : pitches_pyin_d)
		std::cout << "pyin pitch candidate: " << pyin_candidate.first << ","
		          << pyin_candidate.second << std::endl;

	// float + function
	std::vector<float> test_data_f(8092, 5000.0);
	auto pitch_mpm_f = pitch::mpm(test_data_f, 48000);
	auto pitch_yin_f = pitch::yin(test_data_f, 48000);

	auto pitches_pmpm_f = pitch::pmpm(test_data_f, 48000);
	auto pitches_pyin_f = pitch::pyin(test_data_f, 48000);

	std::cout << "mpm pitch: " << pitch_mpm_f << std::endl;
	std::cout << "yin pitch: " << pitch_yin_f << std::endl;

	for (auto pmpm_candidate : pitches_pmpm_f)
		std::cout << "pmpm pitch candidate: " << pmpm_candidate.first << ","
		          << pmpm_candidate.second << std::endl;

	for (auto pyin_candidate : pitches_pyin_f)
		std::cout << "pyin pitch candidate: " << pyin_candidate.first << ","
		          << pyin_candidate.second << std::endl;

	// float + classes
	pitch_alloc::Mpm<float> ma_f(8092);
	pitch_alloc::Yin<float> ya_f(8092);

	pitch_mpm_f = ma_f.pitch(test_data_f, 48000);
	pitch_yin_f = ya_f.pitch(test_data_f, 48000);

	pitches_pmpm_f = ma_f.probabilistic_pitches(test_data_f, 48000);
	pitches_pyin_f = ya_f.probabilistic_pitches(test_data_f, 48000);

	std::cout << "mpm pitch: " << pitch_mpm_f << std::endl;
	std::cout << "yin pitch: " << pitch_yin_f << std::endl;

	for (auto pmpm_candidate : pitches_pmpm_f)
		std::cout << "pmpm pitch candidate: " << pmpm_candidate.first << ","
		          << pmpm_candidate.second << std::endl;

	for (auto pyin_candidate : pitches_pyin_f)
		std::cout << "pyin pitch candidate: " << pyin_candidate.first << ","
		          << pyin_candidate.second << std::endl;

	return 0;
}
