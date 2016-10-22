#include <iostream>
#include "mpm.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
};

/*hack to make av_err2str work in c++
 * taken from:
https://ffmpeg.org/pipermail/libav-user/2013-January/003458.html
*/
#undef av_err2str
#define av_err2str(errnum) \
av_make_error_string((char*\
)__builtin_alloca(AV_ERROR_MAX_STRING_SIZE),\
AV_ERROR_MAX_STRING_SIZE, errnum)

void read_mp3_file(char *path)
{
	// Initialize FFmpeg
	av_register_all();

	AVFrame* frame = avcodec_alloc_frame();
	if (!frame) {
		std::cout << "Error allocating the frame" << std::endl;
		return;
	}

	AVFormatContext* formatContext = NULL;
	int x = avformat_open_input(&formatContext,
				    path, NULL, NULL);
	if (x != 0) {
		av_free(frame);
		std::cout << "Error opening the file" << std::endl;
		printf("Error: %s\n", av_err2str(x));
		return;
	}

	if (avformat_find_stream_info(formatContext, NULL) < 0) {
		av_free(frame);
		avformat_close_input(&formatContext);
		std::cout << "Error finding the stream info" << std::endl;
		return;
	}

	// Find the audio stream
	AVCodec* cdc = nullptr;
	int streamIndex = av_find_best_stream(formatContext,
					      AVMEDIA_TYPE_AUDIO,
					      -1, -1, &cdc, 0);
	if (streamIndex < 0) {
		av_free(frame);
		avformat_close_input(&formatContext);
		std::cout << "Could not find any audio stream in the file"
			  << std::endl;
		return;
	}

	AVStream* audioStream = formatContext->streams[streamIndex];
	AVCodecContext* codecContext = audioStream->codec;
	codecContext->codec = cdc;

	if (avcodec_open2(codecContext, codecContext->codec, NULL) != 0) {
		av_free(frame);
		avformat_close_input(&formatContext);
		std::cout << "Couldn't open the context with the decoder"
			  << std::endl;
		return;
	}

	AVPacket readingPacket;
	av_init_packet(&readingPacket);

	// Read the packets in a loop
	while (av_read_frame(formatContext, &readingPacket) == 0) {
		if (readingPacket.stream_index == audioStream->index) {
			AVPacket decodingPacket = readingPacket;

			// Audio packets can have multiple audio frames in a single packet
			while (decodingPacket.size > 0) {
				int gotFrame = 0;
				int result = avcodec_decode_audio4
					     (codecContext, frame,
					      &gotFrame, &decodingPacket);

				if (result >= 0 && gotFrame) {
					decodingPacket.size -= result;
					decodingPacket.data += result;
					// We now have a fully decoded audio frame

					double raw_audio[frame->nb_samples / codecContext->channels];
					for (int i = 0;
					     i < frame->nb_samples / codecContext->channels;
					     i += 1) {
						raw_audio[i] = (double) frame->data[0][codecContext->channels*i];
					}

					mpm mpm1 =
						mpm(codecContext->sample_rate,
						    frame->nb_samples / codecContext->channels);

					double pitch = mpm1.get_pitch(raw_audio);
					if (pitch != -1) {
						printf("%f\n", pitch);
					}
				} else {
					decodingPacket.size = 0;
					decodingPacket.data = nullptr;
				}
			}
		}
		av_free_packet(&readingPacket);
	}

	av_free(frame);

	avcodec_close(codecContext);
	avformat_close_input(&formatContext);
}
