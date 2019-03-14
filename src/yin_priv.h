#ifndef YIN_PRIV_H
#define YIN_PRIV_H

#include "pitch_detection.h"

namespace {
	namespace yin_consts
	{
		template <typename T> static const T Threshold = static_cast<T>(0.20);
		template <typename T> static const T Alpha = static_cast<T>(1.0);
		template <typename T> static const T Betas[] = {
			static_cast<T>(18.0),
			static_cast<T>(11.0) + static_cast<T>(1.0)/static_cast<T>(3.0),
			static_cast<T>(8.0)
		};
		template <typename T> static const T Means[] = {
			static_cast<T>(0.10),
			static_cast<T>(0.15),
			static_cast<T>(0.20)
		};
		template <typename T> static const T Pa = static_cast<T>(0.01);
		static const int Threshold_N = 100;
	}
}

template <typename T>
void
cumulative_mean_normalized_difference(std::vector<T> &);

template <typename T>
void
difference(const std::vector<T> &, pitch_alloc::Yin<T> *);

#endif /* YIN_PRIV_H */
