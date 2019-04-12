#include <cfloat>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#include "temporal_pitch_tracking.h"
#include <mlpack/core.hpp>

std::string NOTES[N_NOTES] = {
    "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};

std::vector<double> PITCH_BINS(N_BINS);
std::vector<std::string> NOTE_BINS(N_BINS);

const double A = std::pow(2.0, 1.0 / 12.0);

// 108 bins - C0 -> B8
void
init_pitch_bins()
{
	int curr_octave = -1;
	for (int i = 0; i < N_BINS; ++i) {
		auto octave = i % N_NOTES;
		if ((i % N_NOTES) == 0)
			curr_octave++;
		std::ostringstream note_name;
		note_name << NOTES[octave] << curr_octave;
		auto fi = F0 * std::pow(A, i - NOTE_OFFSET);
		PITCH_BINS[i] = fi;
		NOTE_BINS[i] = note_name.str();
	}
}

const arma::rowvec
bin_pitches(const std::vector<std::pair<float, float>> pitch_candidates)
{
	arma::rowvec ret(2 * N_BINS + 1, arma::fill::zeros);

	double prob_pitched = 0.0;

	for (auto pitch_candidate : pitch_candidates) {
		// find the most appropriate bin
		double delta = DBL_MAX;
		double prev_delta = DBL_MAX;
		// int best_pitch_bin_index = -1;
		for (int i = 0; i < N_BINS; ++i) {
			delta = std::abs(pitch_candidate.first - PITCH_BINS[i]);
			if (prev_delta < delta) {
				ret[i - 1] = pitch_candidate.second;
				prob_pitched += ret[i - 1];
				break;
			}
			prev_delta = delta;
		}
	}

	double prob_really_pitched = YIN_TRUST * prob_pitched;

	for (int i = 0; i < N_BINS; ++i) {
		if (prob_pitched > 0) {
			ret[i] *= prob_really_pitched / prob_pitched;
		}
		ret[i + N_BINS] = (1 - prob_really_pitched) / N_BINS;
	}

	return ret;
}
