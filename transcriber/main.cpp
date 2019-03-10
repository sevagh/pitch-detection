#include <libnyquist/Decoders.h>
#include <pitch_detection.h>

int
main(int argc, char **argv)
{
	nqr::NyquistIO loader;

	if (argc != 2) {
		std::cerr << "Usage: transcriber /path/to/audio/file" << std::endl;
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
	}
}
