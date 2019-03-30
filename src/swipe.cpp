#include <iostream>
#include <vector>

#include <algorithm>
#include <climits>
#include <cmath>

#include <complex>
#include <ffts/ffts.h>

#include "pitch_detection/pitch_detection.h"

namespace
{
namespace swipe_consts
{
template <typename T> static const T Derbs = static_cast<T>(0.1);
template <typename T> static const T Polyv = static_cast<T>(0.0013028);
template <typename T> static const T Dlog2p = static_cast<T>(0.0104167);
template <typename T> static const T Dt = static_cast<T>(0.001);
template <typename T> static const T St = static_cast<T>(0.3);
template <typename T> static const T Min = static_cast<T>(10.0);
template <typename T> static const T Max = static_cast<T>(8000.0);
template <typename T> static const T Yp1 = static_cast<T>(2.0);
template <typename T> static const T Ypn = static_cast<T>(2.0);
} // namespace swipe_consts
} // namespace

extern "C" {
extern int
dgels_(char *trans, int *m, int *n, int *nrhs, double *a, int *lda, double *b,
    int *ldb, double *work, int *lwork, int *info);
}

template <typename T>
static int
bilookv(std::vector<T> &yr_vector, T key, size_t lo)
{
	int md;
	size_t hi = yr_vector.size();
	lo--;
	while (hi - lo > 1) {
		md = (hi + lo) >> 1;
		if (yr_vector[md] > key)
			hi = md;
		else
			lo = md;
	}
	return (hi);
}

template <typename T>
static int
bisectv(std::vector<T> &yr_vector, T key)
{
	return bilookv(yr_vector, key, 2);
}

template <typename T> using matrix = std::vector<std::vector<T>>;

static int
sieve(std::vector<int> &ones)
{
	int k = 0;
	size_t sp = floor(sqrt(ones.size()));
	ones[0] = 0;
	for (size_t i = 1; i < sp; i++) {
		if (ones[i] == 1) {
			for (size_t j = i + i + 1; j < ones.size(); j += i + 1) {
				ones[j] = 0;
			}
			k++;
		}
	}
	for (size_t i = sp; i < ones.size(); ++i) {
		if (ones[i] == 1)
			k++;
	}
	return (k);
}

template <typename T>
static void
spline(std::vector<T> &x, std::vector<T> &y, std::vector<T> &y2)
{
	size_t i, j;
	T p, qn, sig;
	std::vector<T> u((unsigned)(x.size() - 1));
	y2[0] = -.5;
	u[0] = (3. / (x[1] - x[0])) *
	       ((y[1] - y[0]) / (x[1] - x[0]) - swipe_consts::Yp1<T>);
	for (i = 1; i < x.size() - 1; i++) {
		sig = (x[i] - x[i - 1]) / (x[i + 1] - x[i - 1]);
		p = sig * y2[i - 1] + 2.;
		y2[i] = (sig - 1.) / p;
		u[i] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]) -
		       (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
		u[i] = (6 * u[i] / (x[i + 1] - x[i - 1]) - sig * u[i - 1]) / p;
	}
	qn = .5;
	y2[y2.size() - 1] = ((3. / (x[x.size() - 1] - x[x.size() - 2])) *
	                            (swipe_consts::Ypn<T> -
	                                (y[y.size() - 1] - y[y.size() - 2]) /
	                                    (x[x.size() - 1] - x[x.size() - 2])) -
	                        qn * u[x.size() - 2]) /
	                    (qn * y2[y2.size() - 2] + 1.);
	for (j = x.size() - 2; j != (size_t)-1; --j)
		y2[j] = y2[j] * y2[j + 1] + u[j];
	return;
}

template <typename T>
static T
splinv(std::vector<T> &x, std::vector<T> &y, std::vector<T> &y2, T val, int hi)
{
	int lo = hi - 1;
	T h = x[hi] - x[lo];
	T a = (x[hi] - val) / h;
	T b = (val - x[lo]) / h;
	return (
	    a * y[lo] + b * y[hi] +
	    ((a * a * a - a) * y2[lo] * (b * b * b - b) * y2[hi]) * (h * h) / 6.);
}

template <typename T>
static void
polyfit(
    std::vector<T> &A, std::vector<T> &B, std::vector<double> &Bp, int degree)
{
	int info;
	degree++;

	// needs to be a double for LAPACK's DGELS
	std::vector<double> Ap(degree * A.size());

	size_t i, j;
	for (i = 0; i < (size_t)degree; i++)
		for (j = 0; j < A.size(); j++)
			Ap[i * A.size() + j] = pow(A[j], degree - i - 1);
	for (i = 0; i < B.size(); i++)
		Bp[i] = B[i];
	i = 1;
	j = A.size() + degree;
	int a_size = (int)A.size();
	int b_size = (int)B.size();
	int int_i = (int)i;
	int int_j = (int)j;

	// needs to be a double for LAPACK's DGELS
	std::vector<double> work(j);

	dgels_((char *)"N", &a_size, &degree, &int_i, Ap.data(), &b_size, Bp.data(),
	    &degree, work.data(), &int_j, &info);
	if (info < 0) {
		fprintf(stderr, "LAPACK routine dgels() returned error: %d\n", info);
		exit(EXIT_FAILURE);
	}
	return;
}

template <typename T>
static T
polyval(std::vector<double> &coefs, T val)
{
	T sum = 0.;
	for (size_t i = 0; i < coefs.size(); i++)
		sum += coefs[i] * pow(val, coefs.size() - i - 1);
	return (sum);
}

template <typename T>
static T
hz2erb(T hz)
{
	return static_cast<T>(21.4 * log10(1. + hz / 229.));
}

template <typename T>
static T
erb2hz(T erb)
{
	return static_cast<T>((pow(10, erb / 21.4) - 1.) * 229.);
}

template <typename T>
static T
fixnan(T x)
{
	return (std::isnan(x) ? 0. : x);
}

template <typename T>
static void
La(matrix<T> &L, std::vector<T> &f, std::vector<T> &fERBs,
    std::vector<std::complex<float>> &fo, int w2, int hi, int i)
{
	size_t j;
	std::vector<T> a(w2);
	for (j = 0; j < (size_t)w2; j++)
		a[j] = sqrt(std::real(fo[j]) * std::real(fo[j]) +
		            std::imag(fo[j]) * std::imag(fo[j]));
	std::vector<T> a2(f.size());
	spline(f, a, a2);
	L[i][0] = fixnan(sqrt(splinv(f, a, a2, fERBs[0], hi)));
	for (j = 1; j < L[0].size(); j++) {
		hi = bilookv(f, fERBs[j], hi);
		L[i][j] = fixnan(sqrt(splinv(f, a, a2, fERBs[j], hi)));
	}
}

template <typename T>
static matrix<T>
loudness(
    const std::vector<T> &x, std::vector<T> &fERBs, T nyquist, int w, int w2)
{
	size_t i, j;
	int hi;
	int offset = 0;
	T td = nyquist / w2;

	// need to be floats for ffts
	std::vector<std::complex<float>> fi(w);
	std::vector<std::complex<float>> fo(w);
	ffts_plan_t *plan = ffts_init_1d(w, FFTS_FORWARD);
	std::vector<T> hann(w);
	for (i = 0; i < (size_t)w; i++)
		hann[i] = .5 - (.5 * cos(2. * M_PI * ((T)i / w)));
	std::vector<T> f(w2);
	for (i = 0; i < (size_t)w2; i++)
		f[i] = i * td;
	hi = bisectv(f, fERBs[0]);
	matrix<T> L(ceil((T)x.size() / w2) + 1, std::vector<T>(fERBs.size()));
	for (j = 0; j < (size_t)w2; j++)
		fi[j] = {0., 0.};
	for (/* j = w2 */; j < (size_t)w; j++)
		fi[j] = {(float)(x[j - w2] * hann[j]), 0.};
	ffts_execute(plan, fi.data(), fo.data());
	La(L, f, fERBs, fo, w2, hi, 0);
	for (i = 1; i < L.size() - 2; i++) {
		for (j = 0; j < (size_t)w; j++)
			fi[j] = {(float)(x[j + offset] * hann[j]), 0.};
		ffts_execute(plan, fi.data(), fo.data());
		La(L, f, fERBs, fo, w2, hi, i);
		offset += w2;
	}
	for (/* i = L.size() - 2; */; i < L.size(); i++) {
		for (j = 0; j < x.size() - offset; j++)
			fi[j] = {(float)(x[j + offset] * hann[j]), 0.};
		for (/* j = x.size() - offset */; j < (size_t)w; j++)
			fi[j] = {0., 0.};
		ffts_execute(plan, fi.data(), fo.data());
		La(L, f, fERBs, fo, w2, hi, i);
		offset += w2;
	}
	for (i = 0; i < L.size(); i++) {
		td = 0.;
		for (j = 0; j < L[0].size(); j++)
			td += L[i][j] * L[i][j];
		if (td != 0.) {
			td = sqrt(td);
			for (j = 0; j < L[0].size(); j++)
				L[i][j] /= td;
		}
	}
	ffts_free(plan);
	return L;
}

template <typename T>
static void
Sadd(matrix<T> &S, matrix<T> &L, std::vector<T> &fERBs, std::vector<T> &pci,
    std::vector<T> &mu, std::vector<int> &ps, T dt, T nyquist2, int lo, int psz,
    int w2)
{
	size_t i, j, k;
	T t = 0.;
	T tp = 0.;
	T td;
	T dtp = w2 / nyquist2;

	matrix<T> Slocal(psz, std::vector<T>(L.size()));
	for (i = 0; i < Slocal.size(); i++) {
		std::vector<T> q(fERBs.size());
		for (j = 0; j < q.size(); j++)
			q[j] = fERBs[j] / pci[i];
		std::vector<T> kernel(fERBs.size());
		for (j = 0; j < ps.size(); j++) {
			if (ps[j] == 1) {
				for (k = 0; k < kernel.size(); k++) {
					td = fabs(q[k] - j - 1.);
					if (td < .25)
						kernel[k] = cos(2. * M_PI * q[k]);
					else if (td < .75)
						kernel[k] += cos(2. * M_PI * q[k]) / 2.;
				}
			}
		}
		td = 0.;
		for (j = 0; j < kernel.size(); j++) {
			kernel[j] *= sqrt(1. / fERBs[j]);
			if (kernel[j] > 0.)
				td += kernel[j] * kernel[j];
		}
		td = sqrt(td);
		for (j = 0; j < kernel.size(); j++)
			kernel[j] /= td;
		for (j = 0; j < L.size(); j++) {
			for (k = 0; k < L[0].size(); k++)
				Slocal[i][j] += kernel[k] * L[j][k];
		}
	}
	k = 0;
	for (j = 0; j < S[0].size(); j++) {
		td = t - tp;
		while (td >= 0.) {
			k++;
			tp += dtp;
			td -= dtp;
		}
		for (int i = 0; i < psz; i++) {
			S[lo + i][j] +=
			    (Slocal[i][k] +
			        (td * (Slocal[i][k] - Slocal[i][k - 1])) / dtp) *
			    mu[i];
		}
		t += dt;
	}
}

template <typename T>
static void
Sfirst(matrix<T> &S, const std::vector<T> &x, std::vector<T> &pc,
    std::vector<T> &fERBs, std::vector<T> &d, std::vector<int> &ws,
    std::vector<int> &ps, T nyquist, T nyquist2, T dt, int n)
{
	int i;
	int w2 = ws[n] / 2;
	matrix<T> L = loudness(x, fERBs, nyquist, ws[n], w2);
	int lo = 0;
	int hi = bisectv(d, static_cast<T>(2.));
	int psz = hi - lo;
	std::vector<T> mu(psz);
	std::vector<T> pci(psz);
	for (i = 0; i < hi; i++) {
		pci[i] = pc[i];
		mu[i] = 1. - fabs(d[i] - 1.);
	}
	Sadd(S, L, fERBs, pci, mu, ps, dt, nyquist2, lo, psz, w2);
}

template <typename T>
static void
Snth(matrix<T> &S, const std::vector<T> &x, std::vector<T> &pc,
    std::vector<T> &fERBs, std::vector<T> &d, std::vector<int> &ws,
    std::vector<int> &ps, T nyquist, T nyquist2, T dt, int n)
{
	int i;
	int w2 = ws[n] / 2;
	matrix<T> L = loudness(x, fERBs, nyquist, ws[n], w2);
	int lo = bisectv(d, static_cast<T>(n));
	int hi = bisectv(d, static_cast<T>(n + 2));
	int psz = hi - lo;
	std::vector<T> mu(psz);
	std::vector<T> pci(psz);
	int ti = 0;
	for (i = lo; i < hi; i++) {
		pci[ti] = pc[i];
		mu[ti] = 1. - fabs(d[i] - (n + 1));
		ti++;
	}
	Sadd(S, L, fERBs, pci, mu, ps, dt, nyquist2, lo, psz, w2);
}

template <typename T>
static void
Slast(matrix<T> &S, const std::vector<T> &x, std::vector<T> &pc,
    std::vector<T> &fERBs, std::vector<T> &d, std::vector<int> &ws,
    std::vector<int> &ps, T nyquist, T nyquist2, T dt, int n)
{
	int i;
	int w2 = ws[n] / 2;
	matrix<T> L = loudness(x, fERBs, nyquist, ws[n], w2);
	int lo = bisectv(d, static_cast<T>(n));
	int hi = d.size();
	int psz = hi - lo;
	std::vector<T> mu(psz);
	std::vector<T> pci(psz);
	int ti = 0;
	for (i = lo; i < hi; i++) {
		pci[ti] = pc[i];
		mu[ti] = 1. - fabs(d[i] - (n + 1));
		ti++;
	}
	Sadd(S, L, fERBs, pci, mu, ps, dt, nyquist2, lo, psz, w2);
}

template <typename T>
static T
pitch_(matrix<T> &S, std::vector<T> &pc, T st)
{
	size_t i, j;
	size_t maxi = (size_t)-1;
	int search = (int)std::round(
	    (std::log2(pc[2]) - std::log2(pc[0])) / swipe_consts::Polyv<T> + 1.);
	T nftc, maxv, log2pc;
	T tc2 = 1. / pc[1];

	std::vector<T> s(3);
	std::vector<T> ntc(3);
	ntc[0] = ((1. / pc[0]) / tc2 - 1.) * 2. * M_PI;
	ntc[1] = (tc2 / tc2 - 1.) * 2. * M_PI;
	ntc[2] = ((1. / pc[2]) / tc2 - 1.) * 2. * M_PI;
	std::vector<T> p;
	for (j = 0; j < S[0].size(); j++) {
		maxv = SHRT_MIN;
		for (i = 0; i < S.size(); i++) {
			if (S[i][j] > maxv) {
				maxv = S[i][j];
				maxi = i;
			}
		}
		if (maxv > st) {
			if (!(maxi == 0 || maxi == S.size() - 1)) {
				tc2 = 1. / pc[maxi];
				log2pc = std::log2(pc[maxi - 1]);
				s[0] = S[maxi - 1][j];
				s[1] = S[maxi][j];
				s[2] = S[maxi + 1][j];
				// needs to be double for LAPACK's DGELS
				std::vector<double> coefs(2 >= s.size() ? 2 : s.size());
				polyfit(ntc, s, coefs, 2);
				maxv = SHRT_MIN;
				for (i = 0; i < (size_t)search; i++) {
					nftc = polyval(coefs,
					    static_cast<T>(
					        ((1. /
					             pow(2, i * swipe_consts::Polyv<T> + log2pc)) /
					                tc2 -
					            1) *
					        (2 * M_PI)));
					if (nftc > maxv) {
						maxv = nftc;
						maxi = i;
					}
				}
				p.push_back(pow(2, log2pc + (maxi * swipe_consts::Polyv<T>)));
			}
		}
	}

	if (p.size() > 0) {
		size_t mean_idx = p.size() - (p.size() / 2) + (p.size() % 2);
		std::sort(p.begin(), p.end());
		return p[mean_idx];
	} else {
		return -1.0;
	}
}

template <typename T>
T
pitch::swipe(const std::vector<T> &x, int samplerate)
{
	size_t i;
	T td = 0.;
	T nyquist = samplerate / 2.;
	T nyquist2 = samplerate;
	T nyquist16 = samplerate * 8.;
	std::vector<int> ws(
	    std::round(std::log2((nyquist16) / swipe_consts::Min<T>) -
	               std::log2((nyquist16) / swipe_consts::Max<T>)) +
	    1);
	for (i = 0; i < ws.size(); ++i)
		ws[i] =
		    pow(2, std::round(std::log2(nyquist16 / swipe_consts::Min<T>))) /
		    pow(2, i);
	std::vector<T> pc(ceil(
	    (std::log2(swipe_consts::Max<T>) - std::log2(swipe_consts::Min<T>)) /
	    swipe_consts::Dlog2p<T>));
	std::vector<T> d(pc.size());
	for (i = pc.size() - 1; i != (size_t)-1; i--) {
		td = std::log2(swipe_consts::Min<T>) + (i * swipe_consts::Dlog2p<T>);
		pc[i] = pow(2, td);
		d[i] = 1. + td - std::log2(nyquist16 / ws[0]);
	}
	std::vector<T> fERBs(ceil(
	    (hz2erb(nyquist) - hz2erb(pow(2, td) / 4)) / swipe_consts::Derbs<T>));
	td = hz2erb(swipe_consts::Min<T> / 4.);
	for (i = 0; i < fERBs.size(); i++)
		fERBs[i] = erb2hz(td + (i * swipe_consts::Derbs<T>));
	std::vector<int> ps(floor(fERBs[fERBs.size() - 1] / pc[0] - .75), 1);
	sieve(ps);
	ps[0] = 1;
	matrix<T> S(pc.size(),
	    std::vector<T>(size_t(
	        ceil(static_cast<T>(x.size()) / nyquist2 / swipe_consts::Dt<T>))));
	Sfirst(
	    S, x, pc, fERBs, d, ws, ps, nyquist, nyquist2, swipe_consts::Dt<T>, 0);
	for (i = 1; i < ws.size() - 1; ++i)
		Snth(S, x, pc, fERBs, d, ws, ps, nyquist, nyquist2, swipe_consts::Dt<T>,
		    i);
	Slast(
	    S, x, pc, fERBs, d, ws, ps, nyquist, nyquist2, swipe_consts::Dt<T>, i);
	auto ret = pitch_(S, pc, swipe_consts::St<T>);

	return ret;
}

template double
pitch::swipe<double>(const std::vector<double> &audio_buffer, int sample_rate);

template float
pitch::swipe<float>(const std::vector<float> &audio_buffer, int sample_rate);
