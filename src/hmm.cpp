#include "pitch_detection_priv.h"
#include <iostream>
#include <mlpack/core.hpp>
#include <mlpack/methods/gmm/gmm.hpp>
#include <mlpack/methods/hmm/hmm.hpp>

namespace
{
namespace hmm_consts
{
static const int Num_Bins_Per_Semitone = 5;
template <typename T> static const T Self_Trans = static_cast<T>(0.99);
template <typename T> static const T Min_Freq = static_cast<T>(61.735);
template <typename T> static const T Yin_Trust = static_cast<T>(0.5);
} // namespace hmm_consts
} // namespace

namespace
{
namespace pyin_hmm
{
template <typename T> class PYinHMM
{
  public:
	int num_bins_per_semitone;
	size_t n_pitches;
	size_t transition_width;

	T self_trans;
	T min_freq;
	T yin_trust;

	std::vector<T> freqs;

	mlpack::hmm::HMM<mlpack::distribution::DiscreteDistribution> hmm;

	PYinHMM(int num_bins_per_semitone, T self_trans, T min_freq, T yin_trust)
	    : num_bins_per_semitone(num_bins_per_semitone),
	      n_pitches(69 * num_bins_per_semitone),
	      transition_width(5 * (num_bins_per_semitone / 2) + 1),
	      min_freq(min_freq), yin_trust(yin_trust)
	{
		std::vector<double> trans_prob;
		freqs = std::vector<T>(2 * n_pitches);

		for (size_t i_pitch = 0; i_pitch < n_pitches; ++i_pitch) {
			int theoretical_min_next_pitch =
			    static_cast<int>(i_pitch) -
			    static_cast<int>(transition_width / 2.0);
			size_t min_next_pitch = i_pitch > transition_width / 2
			                            ? i_pitch - transition_width / 2
			                            : 0;
			size_t max_next_pitch = i_pitch < n_pitches - transition_width / 2
			                            ? i_pitch + transition_width / 2
			                            : n_pitches - 1;

			T weight_sum = 0.0;
			std::vector<T> weights;
			for (size_t i = min_next_pitch; i <= max_next_pitch; ++i) {
				weights.push_back(i <= i_pitch
				                      ? i - theoretical_min_next_pitch + 1
				                      : i_pitch - theoretical_min_next_pitch +
				                            1 - (i - i_pitch));
				weight_sum += weights[weights.size() - 1];
			}

			for (size_t i = min_next_pitch; i <= max_next_pitch; ++i) {
				trans_prob.push_back(
				    weights[i - min_next_pitch] / weight_sum * self_trans);

				trans_prob.push_back(weights[i - min_next_pitch] / weight_sum *
				                     (1 - self_trans));

				trans_prob.push_back(
				    weights[i - min_next_pitch] / weight_sum * self_trans);

				trans_prob.push_back(weights[i - min_next_pitch] / weight_sum *
				                     (1 - self_trans));
			}
		}

		std::vector<double> _initial(2 * n_pitches + 1, 1.0 / 2.0 * n_pitches);
		std::cout << "INITIAL: " << _initial.size() << std::endl;
		std::cout << "TRANSITION: " << trans_prob.size() << std::endl;

		arma::Col<double> initial(_initial);
		arma::Row<double> _transition(trans_prob);
		arma::Mat<double> transition(_transition);
		std::vector<mlpack::distribution::DiscreteDistribution> emission(
		    2*n_pitches + 1, mlpack::distribution::DiscreteDistribution(2 * n_pitches + 1));
		hmm = mlpack::hmm::HMM<mlpack::distribution::DiscreteDistribution>(
		    initial, transition, emission);
	};

	const std::vector<size_t>
	calculate_obs_prob(const std::vector<std::pair<T, T>> pitch_prob)
	{
		std::vector<double> out = std::vector<double>(2 * n_pitches + 1);
		T prob_yin_pitched = static_cast<T>(0.0);

		for (size_t i_pair = 0; i_pair < pitch_prob.size(); ++i_pair) {
			T freq = 440. * std::pow(2, (pitch_prob[i_pair].first - 69) / 12);
			if (freq <= min_freq)
				continue;
			T d = 0.0;
			T oldd = 1000.0;
			for (size_t i_pitch = 0; i_pitch < n_pitches; ++i_pitch) {
				d = std::abs(freq - freqs[i_pitch]);
				if (oldd < d && i_pitch > 0) {
					out[i_pitch - 1] = pitch_prob[i_pair].second;
					prob_yin_pitched += out[i_pitch - 1];
					break;
				}
				oldd = d;
			}
		}

		T prob_really_pitched = yin_trust * prob_yin_pitched;
		for (size_t i_pitch = 0; i_pitch < n_pitches; ++i_pitch) {
			if (prob_yin_pitched > 0)
				out[i_pitch] *= (prob_really_pitched / prob_yin_pitched);
			out[i_pitch + n_pitches] = (1 - prob_really_pitched) / n_pitches;
		}

		arma::Row<double> _obs_prob(out);
		const arma::Mat<double> obs_prob(_obs_prob);
		arma::Row<size_t> viterbi_out;

		auto ret = hmm.Predict(obs_prob, viterbi_out);
		std::cout << "RET: " << ret << std::endl;

		return arma::conv_to<std::vector<size_t>>::from(viterbi_out);
	}
};
} // namespace pyin_hmm
} // namespace

template <typename T>
const std::vector<size_t>
pitch_alloc::PYin<T>::calculate_obs_prob(
    const std::vector<std::pair<T, T>> pitch_prob)
{
	return (std::any_cast<pyin_hmm::PYinHMM<T>>(hmm))
	    .calculate_obs_prob(pitch_prob);
}

template <typename T>
void
init_hmm(pitch_alloc::PYin<T> *pya)
{
	pya->hmm = std::any(pyin_hmm::PYinHMM<T>(hmm_consts::Num_Bins_Per_Semitone,
	    hmm_consts::Self_Trans<T>, hmm_consts::Min_Freq<T>,
	    hmm_consts::Yin_Trust<T>));

	auto x = std::any_cast<pyin_hmm::PYinHMM<T>>(pya->hmm);
}

template void
init_hmm(pitch_alloc::PYin<float> *pya);
template void
init_hmm(pitch_alloc::PYin<double> *pya);

template const std::vector<size_t>
pitch_alloc::PYin<double>::calculate_obs_prob(
    const std::vector<std::pair<double, double>> pitch_prob);

template const std::vector<size_t>
pitch_alloc::PYin<float>::calculate_obs_prob(
    const std::vector<std::pair<float, float>> pitch_prob);

template class pyin_hmm::PYinHMM<double>;
template class pyin_hmm::PYinHMM<float>;
