/* note the leading 'c' ! */
#include "pitch_detection/cpitch_detection.h"
#include <stdio.h>

int
main(int argc, char **argv)
{
	//double + function
	double test_data_d[8092];
	for (int i = 0; i < 8092; ++i)
		test_data_d[i] = 5000.0;

	double mpm_d = pitch_mpm_d(test_data_d, 8092, 48000);
	double yin_d = pitch_mpm_d(test_data_d, 8092, 48000);

	struct pitch_candidates_d_t * pitches_pmpm_d = pitch_pmpm_d(test_data_d, 8092, 48000);
	struct pitch_candidates_d_t * pitches_pyin_d = pitch_pyin_d(test_data_d, 8092, 48000);

	printf("mpm pitch: %f\n", mpm_d);
	printf("yin pitch: %f\n", yin_d);

	for (int i = 0; i < pitches_pmpm_d->n_candidates; i++)
		printf("pmpm pitch candidate: %f, %f\n", pitches_pmpm_d->candidates[i].pitch, pitches_pmpm_d->candidates[i].probability);

	for (int i = 0; i < pitches_pyin_d->n_candidates; i++)
		printf("pyin pitch candidate: %f, %f\n", pitches_pyin_d->candidates[i].pitch, pitches_pyin_d->candidates[i].probability);

	//double + struct
	struct Mpm_d_t *Mpm_d = NewMpmD(8092);
	struct Yin_d_t *Yin_d = NewYinD(8092);

	mpm_d = pitch_alloc_mpm_d(Mpm_d, test_data_d, 48000);
	yin_d = pitch_alloc_yin_d(Yin_d, test_data_d, 48000);

	pitches_pmpm_d = pitch_alloc_pmpm_d(Mpm_d, test_data_d, 48000);
	pitches_pyin_d = pitch_alloc_pyin_d(Yin_d, test_data_d, 48000);

	printf("mpm pitch: %f\n", mpm_d);
	printf("yin pitch: %f\n", yin_d);

	for (int i = 0; i < pitches_pmpm_d->n_candidates; i++)
		printf("pmpm pitch candidate: %f, %f\n", pitches_pmpm_d->candidates[i].pitch, pitches_pmpm_d->candidates[i].probability);

	for (int i = 0; i < pitches_pyin_d->n_candidates; i++)
		printf("pyin pitch candidate: %f, %f\n", pitches_pyin_d->candidates[i].pitch, pitches_pyin_d->candidates[i].probability);

	DeleteMpmD(Mpm_d);
	DeleteYinD(Yin_d);

	//float + function
	float test_data_f[8092];
	for (int i = 0; i < 8092; ++i)
		test_data_f[i] = 5000.0;

	float mpm_f = pitch_mpm_f(test_data_f, 8092, 48000);
	float yin_f = pitch_mpm_f(test_data_f, 8092, 48000);

	struct pitch_candidates_f_t * pitches_pmpm_f = pitch_pmpm_f(test_data_f, 8092, 48000);
	struct pitch_candidates_f_t * pitches_pyin_f = pitch_pyin_f(test_data_f, 8092, 48000);

	printf("mpm pitch: %f\n", mpm_f);
	printf("yin pitch: %f\n", yin_f);

	for (int i = 0; i < pitches_pmpm_f->n_candidates; i++)
		printf("pmpm pitch candidate: %f, %f\n", pitches_pmpm_f->candidates[i].pitch, pitches_pmpm_f->candidates[i].probability);

	for (int i = 0; i < pitches_pyin_f->n_candidates; i++)
		printf("pyin pitch candidate: %f, %f\n", pitches_pyin_f->candidates[i].pitch, pitches_pyin_f->candidates[i].probability);

	//float + struct
	struct Mpm_f_t *Mpm_f = NewMpmF(8092);
	struct Yin_f_t *Yin_f = NewYinF(8092);

	mpm_d = pitch_alloc_mpm_f(Mpm_f, test_data_f, 48000);
	yin_d = pitch_alloc_yin_f(Yin_f, test_data_f, 48000);

	pitches_pmpm_f = pitch_alloc_pmpm_f(Mpm_f, test_data_f, 48000);
	pitches_pyin_f = pitch_alloc_pyin_f(Yin_f, test_data_f, 48000);

	printf("mpm pitch: %f\n", mpm_f);
	printf("yin pitch: %f\n", yin_f);

	for (int i = 0; i < pitches_pmpm_f->n_candidates; i++)
		printf("pmpm pitch candidate: %f, %f\n", pitches_pmpm_f->candidates[i].pitch, pitches_pmpm_f->candidates[i].probability);

	for (int i = 0; i < pitches_pyin_f->n_candidates; i++)
		printf("pyin pitch candidate: %f, %f\n", pitches_pyin_f->candidates[i].pitch, pitches_pyin_f->candidates[i].probability);

	DeleteMpmF(Mpm_f);
	DeleteYinF(Yin_f);

	return 0;
}
