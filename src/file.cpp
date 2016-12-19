#include <iostream>
#include "file.hpp"
#include "pitch.hpp"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

void read_audio_file(std::string path, std::string algo)
{
	av_register_all();

	AVFrame* frame = av_frame_alloc();
	AVFormatContext* fmt_ctx = NULL;
	avformat_open_input(&fmt_ctx, path.c_str(), NULL, NULL);
	avformat_find_stream_info(fmt_ctx, NULL);
	AVCodec* cdc = nullptr;
	int stream_idx = av_find_best_stream(fmt_ctx,
					     AVMEDIA_TYPE_AUDIO,
					     -1, -1, &cdc, 0);
	AVStream* astream = fmt_ctx->streams[stream_idx];
	AVCodecContext* codec_ctx = astream->codec;
	codec_ctx->codec = cdc;
	avcodec_open2(codec_ctx, codec_ctx->codec, NULL);

	AVPacket rpkt;
	av_init_packet(&rpkt);

	int incr = 1000;

	while (av_read_frame(fmt_ctx, &rpkt) == 0) {
		if (rpkt.stream_index == astream->index) {
			AVPacket dpkt = rpkt;

			while (dpkt.size > 0) {
				int gotFrame = 0;
				int result = avcodec_decode_audio4
					     (codec_ctx, frame,
					      &gotFrame, &dpkt);

				if (result >= 0 && gotFrame) {
					dpkt.size -= result;
					dpkt.data += result;
					int l = (double) frame->linesize[0]/ (double) codec_ctx->channels;

					double *raw = new double[l];
					for (int i = 0; i < l; i += 1) {
						raw[i] = (double) frame->
							 data[0][codec_ctx->channels*i];
					}

					int64_t tstamp = (double)
							 frame->best_effort_timestamp * (double) astream->time_base.num / (double) astream->time_base.den * 1000.0;

					for (int j = 0; j < l - incr; j += incr) {
						std::vector<double> chunk(&raw[j], &raw[j+incr]);
						double pitch =
							get_pitch(algo, chunk, codec_ctx->sample_rate);
						if (pitch != -1) {
							printf("tstamp: %ld\t%f\n", tstamp, pitch);
						}
					}
					delete [] raw;
				} else {
					dpkt.size = 0;
					dpkt.data = nullptr;
				}
			}
		}
		av_packet_unref(&rpkt);
	}

	av_free(frame);

	avcodec_close(codec_ctx);
	avformat_close_input(&fmt_ctx);
}
