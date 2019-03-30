#include "pitch_detection/pitch_detection.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <libnyquist/Decoders.h>

int
main(int argc, char **argv)
{
	nqr::NyquistIO loader;

	if (argc != 2) {
		std::cerr << "Usage: wav_analyzer /path/to/audio/file" << std::endl;
		return -1;
	}

	std::shared_ptr<nqr::AudioData> file_data =
	    std::make_shared<nqr::AudioData>();
	loader.Load(file_data.get(), argv[1]);

	std::cout << "Audio file info:" << std::endl;

	std::cout << "\tsample rate: " << file_data->sampleRate << std::endl;
	std::cout << "\tlen samples: " << file_data->samples.size() << std::endl;
	std::cout << "\tframe size: " << file_data->frameSize << std::endl;
	std::cout << "\tseconds: " << file_data->lengthSeconds << std::endl;
	std::cout << "\tchannels: " << file_data->channelCount << std::endl;

	std::transform(file_data->samples.begin(), file_data->samples.end(),
	    file_data->samples.begin(),
	    std::bind(std::multiplies<float>(), std::placeholders::_1, 10000));

	std::vector<float> audio;

	if (file_data->channelCount == 2) {
		// convert stereo to mono
		std::vector<float> audio_copy(file_data->samples.size() / 2);
		nqr::StereoToMono(file_data->samples.data(), audio_copy.data(),
		    file_data->samples.size());
		audio = std::vector<float>(audio_copy.begin(), audio_copy.end());
	} else {
		audio = std::vector<float>(
		    file_data->samples.begin(), file_data->samples.end());
	}

	std::vector<double> audio_(audio.begin(), audio.end());

	auto pitch_mpm = pitch::mpm<float>(audio, file_data->sampleRate);
	auto pitch_yin = pitch::yin<float>(audio, file_data->sampleRate);
	auto pitch_swipe = pitch::swipe(audio_, file_data->sampleRate);

	std::cout << "mpm: " << pitch_mpm << "\nyin: " << pitch_yin
	          << "\nswipe: " << pitch_swipe << std::endl;

	return 0;
}
