#include <benchmark/benchmark.h>
#include <pitch_detection.h>
#include <util.h>

static void
BM_Yin_Sinewave(benchmark::State &state)
{
	auto data = util::sinewave(state.range(0), 1337, 48000);
	for (auto _ : state)
		pitch::yin(data, 48000);
	state.SetComplexityN(state.range(0));
}

static void
BM_Mpm_Sinewave(benchmark::State &state)
{
	auto data = util::sinewave(state.range(0), 1337, 48000);
	for (auto _ : state)
		pitch::mpm(data, 48000);
	state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_Yin_Sinewave)->Range(1 << 10, 1 << 20)->Complexity();

BENCHMARK(BM_Mpm_Sinewave)->Range(1 << 10, 1 << 20)->Complexity();

BENCHMARK_MAIN();
