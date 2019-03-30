#include "pitch_detection/pitch_detection.h"

extern "C" {
#include "pitch_detection/cpitch_detection.h"
#include <string.h>

double
pitch_yin_d(const double *data, long data_size, int sample_rate)
{
	std::vector<double> data_vec(data, data + data_size);

	return pitch::yin(data_vec, sample_rate);
}

float
pitch_yin_f(const float *data, long data_size, int sample_rate)
{
	std::vector<float> data_vec(data, data + data_size);

	return pitch::yin(data_vec, sample_rate);
}

double
pitch_swipe_d(const double *data, long data_size, int sample_rate)
{
	std::vector<double> data_vec(data, data + data_size);

	return pitch::swipe(data_vec, sample_rate);
}

float
pitch_swipe_f(const float *data, long data_size, int sample_rate)
{
	std::vector<float> data_vec(data, data + data_size);

	return pitch::swipe(data_vec, sample_rate);
}

double
pitch_mpm_d(const double *data, long data_size, int sample_rate)
{
	std::vector<double> data_vec(data, data + data_size);

	return pitch::mpm(data_vec, sample_rate);
}

float
pitch_mpm_f(const float *data, long data_size, int sample_rate)
{
	std::vector<float> data_vec(data, data + data_size);

	return pitch::mpm(data_vec, sample_rate);
}

struct pitch_candidates_d_t *
pitch_pyin_d(const double *data, long data_size, int sample_rate)
{
	std::vector<double> data_vec(data, data + data_size);

	auto candidates = pitch::pyin(data_vec, sample_rate);

	struct pitch_candidates_d_t *ret = (struct pitch_candidates_d_t *)malloc(
	    sizeof(struct pitch_candidates_d_t *));
	ret->n_candidates = candidates.size();
	ret->candidates = (struct pitch_probability_pair_d_t *)malloc(
	    ret->n_candidates * sizeof(struct pitch_probability_pair_d_t));

	int index = 0;
	for (auto c : candidates) {
		struct pitch_probability_pair_d_t pair = {
		    .pitch = c.first, .probability = c.second};
		memcpy(&ret->candidates[index++], &pair,
		    sizeof(struct pitch_probability_pair_d_t));
	}

	return ret;
}

struct pitch_candidates_f_t *
pitch_pyin_f(const float *data, long data_size, int sample_rate)
{
	std::vector<float> data_vec(data, data + data_size);

	auto candidates = pitch::pyin(data_vec, sample_rate);

	struct pitch_candidates_f_t *ret = (struct pitch_candidates_f_t *)malloc(
	    sizeof(struct pitch_candidates_f_t *));
	ret->n_candidates = candidates.size();
	ret->candidates = (struct pitch_probability_pair_f_t *)malloc(
	    ret->n_candidates * sizeof(struct pitch_probability_pair_f_t));

	int index = 0;
	for (auto c : candidates) {
		struct pitch_probability_pair_f_t pair = {
		    .pitch = c.first, .probability = c.second};
		memcpy(&ret->candidates[index++], &pair,
		    sizeof(struct pitch_probability_pair_f_t));
	}

	return ret;
}

struct pitch_candidates_d_t *
pitch_pmpm_d(const double *data, long data_size, int sample_rate)
{
	std::vector<double> data_vec(data, data + data_size);

	auto candidates = pitch::pmpm(data_vec, sample_rate);

	struct pitch_candidates_d_t *ret = (struct pitch_candidates_d_t *)malloc(
	    sizeof(struct pitch_candidates_d_t *));
	ret->n_candidates = candidates.size();
	ret->candidates = (struct pitch_probability_pair_d_t *)malloc(
	    ret->n_candidates * sizeof(struct pitch_probability_pair_d_t));

	int index = 0;
	for (auto c : candidates) {
		struct pitch_probability_pair_d_t pair = {
		    .pitch = c.first, .probability = c.second};
		memcpy(&ret->candidates[index++], &pair,
		    sizeof(struct pitch_probability_pair_d_t));
	}

	return ret;
}

struct pitch_candidates_f_t *
pitch_pmpm_f(const float *data, long data_size, int sample_rate)
{
	std::vector<float> data_vec(data, data + data_size);

	auto candidates = pitch::pmpm(data_vec, sample_rate);

	struct pitch_candidates_f_t *ret = (struct pitch_candidates_f_t *)malloc(
	    sizeof(struct pitch_candidates_f_t *));
	ret->n_candidates = candidates.size();
	ret->candidates = (struct pitch_probability_pair_f_t *)malloc(
	    ret->n_candidates * sizeof(struct pitch_probability_pair_f_t));

	int index = 0;
	for (auto c : candidates) {
		struct pitch_probability_pair_f_t pair = {
		    .pitch = c.first, .probability = c.second};
		memcpy(&ret->candidates[index++], &pair,
		    sizeof(struct pitch_probability_pair_f_t));
	}

	return ret;
}

struct Yin_d_t {
	void *obj;
};

struct Yin_f_t {
	void *obj;
};

struct Mpm_d_t {
	void *obj;
};

struct Mpm_f_t {
	void *obj;
};

struct Mpm_d_t *
NewMpmD(long size)
{
	struct Mpm_d_t *ret = (struct Mpm_d_t *)malloc(sizeof(struct Mpm_d_t *));

	ret->obj = static_cast<void *>(new pitch_alloc::Mpm<double>(size));
	return ret;
}

void
DeleteMpmD(struct Mpm_d_t *m)
{
	auto ma = static_cast<pitch_alloc::Mpm<double> *>(m->obj);
	delete ma;

	free(m);
}

struct Mpm_f_t *
NewMpmF(long size)
{
	struct Mpm_f_t *ret = (struct Mpm_f_t *)malloc(sizeof(struct Mpm_f_t *));

	ret->obj = static_cast<void *>(new pitch_alloc::Mpm<float>(size));
	return ret;
}

void
DeleteMpmF(struct Mpm_f_t *m)
{
	auto ma = static_cast<pitch_alloc::Mpm<float> *>(m->obj);
	delete ma;

	free(m);
}

struct Yin_d_t *
NewYinD(long size)
{
	struct Yin_d_t *ret = (struct Yin_d_t *)malloc(sizeof(struct Yin_d_t *));

	ret->obj = static_cast<void *>(new pitch_alloc::Yin<double>(size));

	return ret;
}

void
DeleteYinD(struct Yin_d_t *y)
{
	auto ya = static_cast<pitch_alloc::Yin<double> *>(y->obj);
	delete ya;

	free(y);
}

struct Yin_f_t *
NewYinF(long size)
{
	struct Yin_f_t *ret = (struct Yin_f_t *)malloc(sizeof(struct Yin_f_t *));

	ret->obj = static_cast<void *>(new pitch_alloc::Yin<float>(size));

	return ret;
}

void
DeleteYinF(struct Yin_f_t *y)
{
	auto ya = static_cast<pitch_alloc::Yin<float> *>(y->obj);
	delete ya;

	free(y);
}

struct pitch_candidates_f_t *
pitch_alloc_pmpm_f(struct Mpm_f_t *m, const float *data, int sample_rate)
{
	auto ma = static_cast<pitch_alloc::Mpm<float> *>(m->obj);
	std::vector<float> data_vec(data, data + ma->N);

	auto candidates = ma->probabilistic_pitches(data_vec, sample_rate);

	struct pitch_candidates_f_t *ret = (struct pitch_candidates_f_t *)malloc(
	    sizeof(struct pitch_candidates_f_t *));
	ret->n_candidates = candidates.size();
	ret->candidates = (struct pitch_probability_pair_f_t *)malloc(
	    ret->n_candidates * sizeof(struct pitch_probability_pair_f_t));

	int index = 0;
	for (auto c : candidates) {
		struct pitch_probability_pair_f_t pair = {
		    .pitch = c.first, .probability = c.second};
		memcpy(&ret->candidates[index++], &pair,
		    sizeof(struct pitch_probability_pair_f_t));
	}

	return ret;
}

struct pitch_candidates_f_t *
pitch_alloc_pyin_f(struct Yin_f_t *y, const float *data, int sample_rate)
{
	auto ya = static_cast<pitch_alloc::Yin<float> *>(y->obj);
	std::vector<float> data_vec(data, data + ya->N);

	auto candidates = ya->probabilistic_pitches(data_vec, sample_rate);

	struct pitch_candidates_f_t *ret = (struct pitch_candidates_f_t *)malloc(
	    sizeof(struct pitch_candidates_f_t *));
	ret->n_candidates = candidates.size();
	ret->candidates = (struct pitch_probability_pair_f_t *)malloc(
	    ret->n_candidates * sizeof(struct pitch_probability_pair_f_t));

	int index = 0;
	for (auto c : candidates) {
		struct pitch_probability_pair_f_t pair = {
		    .pitch = c.first, .probability = c.second};
		memcpy(&ret->candidates[index++], &pair,
		    sizeof(struct pitch_probability_pair_f_t));
	}

	return ret;
}

float
pitch_alloc_mpm_f(struct Mpm_f_t *m, const float *data, int sample_rate)
{
	auto ma = static_cast<pitch_alloc::Mpm<float> *>(m->obj);
	std::vector<float> data_vec(data, data + ma->N);

	return ma->pitch(data_vec, sample_rate);
}

float
pitch_alloc_yin_f(struct Yin_f_t *y, const float *data, int sample_rate)
{
	auto ya = static_cast<pitch_alloc::Yin<float> *>(y->obj);
	std::vector<float> data_vec(data, data + ya->N);

	return ya->pitch(data_vec, sample_rate);
}

struct pitch_candidates_d_t *
pitch_alloc_pmpm_d(struct Mpm_d_t *m, const double *data, int sample_rate)
{
	auto ma = static_cast<pitch_alloc::Mpm<double> *>(m->obj);
	std::vector<double> data_vec(data, data + ma->N);

	auto candidates = ma->probabilistic_pitches(data_vec, sample_rate);

	struct pitch_candidates_d_t *ret = (struct pitch_candidates_d_t *)malloc(
	    sizeof(struct pitch_candidates_d_t *));
	ret->n_candidates = candidates.size();
	ret->candidates = (struct pitch_probability_pair_d_t *)malloc(
	    ret->n_candidates * sizeof(struct pitch_probability_pair_d_t));

	int index = 0;
	for (auto c : candidates) {
		struct pitch_probability_pair_d_t pair = {
		    .pitch = c.first, .probability = c.second};
		memcpy(&ret->candidates[index++], &pair,
		    sizeof(struct pitch_probability_pair_d_t));
	}

	return ret;
}

struct pitch_candidates_d_t *
pitch_alloc_pyin_d(struct Yin_d_t *y, const double *data, int sample_rate)
{
	auto ya = static_cast<pitch_alloc::Yin<double> *>(y->obj);
	std::vector<double> data_vec(data, data + ya->N);

	auto candidates = ya->probabilistic_pitches(data_vec, sample_rate);

	struct pitch_candidates_d_t *ret = (struct pitch_candidates_d_t *)malloc(
	    sizeof(struct pitch_candidates_d_t *));
	ret->n_candidates = candidates.size();
	ret->candidates = (struct pitch_probability_pair_d_t *)malloc(
	    ret->n_candidates * sizeof(struct pitch_probability_pair_d_t));

	int index = 0;
	for (auto c : candidates) {
		struct pitch_probability_pair_d_t pair = {
		    .pitch = c.first, .probability = c.second};
		memcpy(&ret->candidates[index++], &pair,
		    sizeof(struct pitch_probability_pair_d_t));
	}

	return ret;
}

double
pitch_alloc_mpm_d(struct Mpm_d_t *m, const double *data, int sample_rate)
{
	auto ma = static_cast<pitch_alloc::Mpm<double> *>(m->obj);
	std::vector<double> data_vec(data, data + ma->N);
	return ma->pitch(data_vec, sample_rate);
}

double
pitch_alloc_yin_d(struct Yin_d_t *y, const double *data, int sample_rate)
{
	auto ya = static_cast<pitch_alloc::Yin<double> *>(y->obj);
	std::vector<double> data_vec(data, data + ya->N);
	return ya->pitch(data_vec, sample_rate);
}
}
