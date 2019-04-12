#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <mlpack/core.hpp>
#include <mlpack/methods/hmm/hmm.hpp>

#include "temporal_pitch_tracking.h"

void
init_pitch_bins();

const arma::vec
bin_pitches(std::vector<std::pair<float, float>>);

class CustomDistribution //: mlpack::distribution::Distribution
{
	public:
		size_t dims;
		CustomDistribution(size_t dims) :
			dims(dims) {}

		double Probability(const arma::mat &observation) const {
			return 1.0;
		}

		size_t Dimensionality() const {
			return dims;
		}
};

mlpack::hmm::HMM<CustomDistribution>
build_hmm()
{
	size_t hmm_size = 2 * N_BINS + 1;
	// initial
	arma::vec initial(hmm_size);
	initial.fill(1.0 / double(hmm_size));

	arma::mat transition(hmm_size, hmm_size, arma::fill::zeros);

	// transitions
	for (int i = 0; i < N_BINS; ++i) {
		int half_transition = static_cast<int>(TRANSITION_WIDTH / 2.0);
		int theoretical_min_next_pitch = i - half_transition;
		int min_next_pitch = i > half_transition ? i - half_transition : 0;
		int max_next_pitch =
		    i < N_BINS - half_transition ? i + half_transition : N_BINS - 1;

		double weight_sum = 0.0;
		std::vector<double> weights;

		for (int j = min_next_pitch; j <= max_next_pitch; ++j) {
			if (j <= i) {
				weights.push_back(j - theoretical_min_next_pitch + 1);
			} else {
				weights.push_back(i - theoretical_min_next_pitch + 1 - j + i);
			}
			weight_sum += weights[weights.size() - 1];
		}

		for (int j = min_next_pitch; j <= max_next_pitch; ++j) {
			transition(i, j) =
			    (weights[j - min_next_pitch] / weight_sum * SELF_TRANS);
			transition(i, j + N_BINS) =
			    (weights[j - min_next_pitch] / weight_sum * (1.0 - SELF_TRANS));
			transition(i + N_BINS, j + N_BINS) =
			    (weights[j - min_next_pitch] / weight_sum * SELF_TRANS);
			transition(i + N_BINS, j) =
			    (weights[j - min_next_pitch] / weight_sum * (1.0 - SELF_TRANS));
		}
	}

	auto hmm = mlpack::hmm::HMM(
	    hmm_size, CustomDistribution(hmm_size));
	hmm.Initial() = initial;
	hmm.Transition() = transition;

	return hmm;
}

double
pitch_from_hmm(
    const std::vector<std::vector<std::pair<float, float>>> pitch_candidates)
{
	init_pitch_bins();

	auto hmm = build_hmm();

	// create observation matrix from consecutive frames
	arma::mat observation(pitch_candidates.size(), 2 * N_BINS + 1);

	for (size_t i = 0; i < pitch_candidates.size(); ++i) {
		auto observation_ = bin_pitches(pitch_candidates[i]);
		observation.row(i) = observation_;
	}

	arma::Row<size_t> state;
	auto viterbi_out = hmm.Predict(observation, state);
	std::cout << "states after viterbi: " << state.size() << std::endl;
	std::cout << "viterbi out: " << viterbi_out << std::endl;

	//for (auto state_ : state) {
	//	std::cout << "state: " << state_ << std::endl;
	//}

	return -1.0;
}
