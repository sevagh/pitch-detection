#include <benchmark/benchmark.h>
#include <pitch_detection.h>
#include <util.h>

static void
BM_Yin_Sinewave_128k_samples(benchmark::State &state)
{
	auto data = util::sinewave(128000, 1337.0, 48000);
	for (auto _ : state)
		pitch::yin(data, 48000);
}

BENCHMARK(BM_Yin_Sinewave_128k_samples);

static void
BM_Mpm_Sinewave_128k_samples(benchmark::State &state)
{
	auto data = util::sinewave(128000, 1337, 48000);
	for (auto _ : state)
		pitch::mpm(data, 48000);
}

BENCHMARK(BM_Mpm_Sinewave_128k_samples);

BENCHMARK_MAIN();
