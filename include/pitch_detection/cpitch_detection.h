#ifndef C_PITCH_DETECTION_H
#define C_PITCH_DETECTION_H

#ifdef __cplusplus
extern "C" {
#endif

struct pitch_candidates_d_t {
	long n_candidates;
	struct pitch_probability_pair_d_t *candidates;
};

struct pitch_candidates_f_t {
	long n_candidates;
	struct pitch_probability_pair_f_t *candidates;
};

struct pitch_probability_pair_d_t {
	double pitch;
	double probability;
};

struct pitch_probability_pair_f_t {
	float pitch;
	float probability;
};

double
pitch_yin_d(const double *, long, int);
float
pitch_yin_f(const float *, long, int);

double
pitch_mpm_d(const double *, long, int);
float
pitch_mpm_f(const float *, long, int);

struct pitch_candidates_d_t *
pitch_pyin_d(const double *, long, int);
struct pitch_candidates_f_t *
pitch_pyin_f(const float *, long, int);

struct pitch_candidates_d_t *
pitch_pmpm_d(const double *, long, int);
struct pitch_candidates_f_t *
pitch_pmpm_f(const float *, long, int);

struct Mpm_d_t;
struct Mpm_f_t;

struct Mpm_d_t *
NewMpmD(long);
void
DeleteMpmD(struct Mpm_d_t *);

struct Mpm_f_t *
NewMpmF(long);
void
DeleteMpmF(struct Mpm_f_t *);

struct Yin_d_t;
struct Yin_f_t;

struct Yin_d_t *
NewYinD(long);
void
DeleteYinD(struct Yin_d_t *);

struct Yin_f_t *
NewYinF(long);
void
DeleteYinF(struct Yin_f_t *);

double
pitch_alloc_yin_d(struct Yin_d_t *, const double *, int);
float
pitch_alloc_yin_f(struct Yin_f_t *, const float *, int);

double
pitch_alloc_mpm_d(struct Mpm_d_t *, const double *, int);
float
pitch_alloc_mpm_f(struct Mpm_f_t *, const float *, int);

struct pitch_candidates_d_t *
pitch_alloc_pyin_d(struct Yin_d_t *, const double *, int);
struct pitch_candidates_f_t *
pitch_alloc_pyin_f(struct Yin_f_t *, const float *, int);

struct pitch_candidates_d_t *
pitch_alloc_pmpm_d(struct Mpm_d_t *, const double *, int);
struct pitch_candidates_f_t *
pitch_alloc_pmpm_f(struct Mpm_f_t *, const float *, int);

#ifdef __cplusplus
}
#endif

#endif /* C_PITCH_DETECTION_H */
