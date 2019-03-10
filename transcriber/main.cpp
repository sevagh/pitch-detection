#include "pitch_detection.h"
#include <libnyquist/Decoders.h>

int
main(int argc, char **argv)
{
	nqr::NyquistIO loader;

	if (argc != 2) {
		std::cerr << "Usage: transcriber /path/to/audio/file" << std::endl;
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

	if (file_data->channelCount == 2) {
		// convert stereo to mono
		std::vector<float> audio_copy(file_data->samples.size() / 2);
		nqr::StereoToMono(file_data->samples.data(), audio_copy.data(),
		    file_data->samples.size());

		auto step_size = file_data->sampleRate;
		for (long unsigned int x = 0; x < file_data->samples.size();
		     x += step_size) {
			std::cout << "sample: " << x << std::endl;
			if (x + step_size >= audio_copy.size()) {
				break;
			}
			pitch::pyin<float>(std::vector<float>(audio_copy.begin() + x, audio_copy.begin() + x + step_size), file_data->sampleRate);
			//std::cout << "mpm pitch: "
			//          << pitch::mpm<float>(
			//                 std::vector<float>(audio_copy.begin() + x,
			//                     audio_copy.begin() + x + step_size),
			//                 file_data->sampleRate)
			//	  << "\t yin: "
			//	  << pitch::yin<float>(
			//                 std::vector<float>(audio_copy.begin() + x,
			//                     audio_copy.begin() + x + step_size),
			//                 file_data->sampleRate)
			//	  << "\t pyin: "
			//	  << pitch::pyin<float>(
			//                 std::vector<float>(audio_copy.begin() + x,
			//                     audio_copy.begin() + x + step_size),
			//                 file_data->sampleRate)
			//          << std::endl;
		}
	}
}
