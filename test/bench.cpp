#include "pitch_detection.h"
#include "util.h"
#include <benchmark/benchmark.h>

static void
BM_Yin_Sinewave(benchmark::State &state)
{
	auto data = test_util::sinewave<float>(state.range(0), 1337, 48000);
	for (auto _ : state)
		pitch::yin(data, 48000);
	state.SetComplexityN(state.range(0));
}

static void
BM_Mpm_Sinewave(benchmark::State &state)
{
	auto data = test_util::sinewave<float>(state.range(0), 1337, 48000);
	for (auto _ : state)
		pitch::mpm(data, 48000);
	state.SetComplexityN(state.range(0));
}

static void
BM_YinFloat_Sinewave_Alloc(benchmark::State &state)
{
	auto data = test_util::sinewave<float>(state.range(0), 1337, 48000);
	pitch_alloc::Yin<float> ya(data.size());
	for (auto _ : state)
		ya.pitch(data, 48000);
	state.SetComplexityN(state.range(0));
}

static void
BM_MpmFloat_Sinewave_Alloc(benchmark::State &state)
{
	auto data = test_util::sinewave<float>(state.range(0), 1337, 48000);
	pitch_alloc::Mpm<float> ma(data.size());
	for (auto _ : state)
		ma.pitch(data, 48000);
	state.SetComplexityN(state.range(0));
}

static void
BM_YinDouble_Sinewave_Alloc(benchmark::State &state)
{
	auto data = test_util::sinewave<double>(state.range(0), 1337, 48000);
	pitch_alloc::Yin<double> ya(data.size());
	for (auto _ : state)
		ya.pitch(data, 48000);
	state.SetComplexityN(state.range(0));
}

static void
BM_MpmDouble_Sinewave_Alloc(benchmark::State &state)
{
	auto data = test_util::sinewave<double>(state.range(0), 1337, 48000);
	pitch_alloc::Mpm<double> ma(data.size());
	for (auto _ : state)
		ma.pitch(data, 48000);
	state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_Yin_Sinewave)->Range(1 << 10, 1 << 20)->Complexity();
BENCHMARK(BM_Mpm_Sinewave)->Range(1 << 10, 1 << 20)->Complexity();

BENCHMARK(BM_YinFloat_Sinewave_Alloc)->Range(1 << 10, 1 << 20)->Complexity();
BENCHMARK(BM_MpmFloat_Sinewave_Alloc)->Range(1 << 10, 1 << 20)->Complexity();

BENCHMARK(BM_YinDouble_Sinewave_Alloc)->Range(1 << 10, 1 << 20)->Complexity();
BENCHMARK(BM_MpmDouble_Sinewave_Alloc)->Range(1 << 10, 1 << 20)->Complexity();

BENCHMARK_MAIN();
