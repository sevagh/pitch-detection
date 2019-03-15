#include "pitch_detection.h"
#include <algorithm>
#include <functional>
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

	std::cout << "sample rate: " << file_data->sampleRate << std::endl;
	std::cout << "len samples: " << file_data->samples.size() << std::endl;
	std::cout << "frame size: " << file_data->frameSize << std::endl;
	std::cout << "seconds: " << file_data->lengthSeconds << std::endl;
	std::cout << "channels: " << file_data->channelCount << std::endl;

	std::transform(file_data->samples.begin(), file_data->samples.end(),
	    file_data->samples.begin(),
	    std::bind(std::multiplies<float>(), std::placeholders::_1, 1000));

	if (file_data->channelCount == 2) {
		// convert stereo to mono
		std::vector<float> audio_copy(file_data->samples.size() / 2);
		nqr::StereoToMono(file_data->samples.data(), audio_copy.data(),
		    file_data->samples.size());

		pitch::pyin<float>(audio_copy, file_data->sampleRate);

		return 0;
	}

	pitch::pyin<float>(file_data->samples, file_data->sampleRate);
	std::cout << "mpm: "
	          << pitch::mpm<float>(
	                 std::vector<float>(file_data->samples.begin() + 48000,
	                     file_data->samples.begin() + 56092),
	                 file_data->sampleRate)
	          << std::endl;
}
