//
// Created by sevag on 4/27/15.
//

#include "mpm.h"
#include <float.h>

#define CUTOFF 0.93 //0.97 is default
#define SMALL_CUTOFF 0.5
#define LOWER_PITCH_CUTOFF 80 //hz

#define MAX(a, b) ((a < b) ?  (b) : (a))

mpm::mpm(double sampling_rate, int size) {
    mpm::sampling_rate = sampling_rate;
    mpm::data_size = size;

    mpm::max_positions = new int[mpm::data_size];
    mpm::period_estimates = new double[mpm::data_size];
    mpm::amp_estimates = new double[mpm::data_size];
}

void mpm::parabolic_interpolation(int tau) {
    double nsdfa = nsdf[tau - 1];
    double nsdfb = nsdf[tau];
    double nsdfc = nsdf[tau + 1];
    double bValue = tau;
    double bottom = nsdfc + nsdfa - 2 * nsdfb;
    if (bottom == 0.0) {
        turning_point_x = bValue;
        turning_point_y = nsdfb;
    } else {
        double delta = nsdfa - nsdfc;
        turning_point_x = bValue + delta / (2 * bottom);
        turning_point_y = nsdfb - delta * delta / (8 * bottom);
    }
}

void mpm::nsdf_time_domain(double *audio_buffer) {
    int tau;
    double nsdf_loc[data_size];
    for (tau = 0; tau < data_size; tau++) {
        double acf = 0;
        double divisorM = 0;
        int i;
        for (i = 0; i < data_size - tau; i++) {
            acf += audio_buffer[i] * audio_buffer[i + tau];
            divisorM += audio_buffer[i] * audio_buffer[i] + audio_buffer[i + tau] * audio_buffer[i + tau];
        }
        nsdf_loc[tau] = 2 * acf / divisorM;
    }
    nsdf = nsdf_loc;
}

void mpm::peak_picking() {
    int pos = 0;
    int curMaxPos = 0;

    while (pos < (data_size - 1) / 3 && nsdf[pos] > 0) {
        pos++;
    }

    while (pos < data_size - 1 && nsdf[pos] <= 0.0) {
        pos++;
    }

    if (pos == 0) {
        pos = 1;
    }

    while (pos < data_size - 1) {
        if (nsdf[pos] > nsdf[pos - 1] && nsdf[pos] >= nsdf[pos + 1]) {
            if (curMaxPos == 0) {
                curMaxPos = pos;
            } else if (nsdf[pos] > nsdf[curMaxPos]) {
                curMaxPos = pos;
            }
        }
        pos++;
        if (pos < data_size - 1 && nsdf[pos] <= 0) {
            if (curMaxPos > 0) {
                max_positions[max_positions_ptr++] = curMaxPos;
                curMaxPos = 0;
            }
            while (pos < data_size - 1 && nsdf[pos] <= 0.0f) {
                pos++;
            }
        }
    }
    if (curMaxPos > 0) {
        max_positions[max_positions_ptr++] = curMaxPos;
    }
}

double mpm::get_pitch(double *audio_buffer) {
    double pitch;

    max_positions_ptr = 0;
    period_estimates_ptr = 0;
    amp_estimates_ptr = 0;

    mpm::nsdf_time_domain(audio_buffer);

    peak_picking();

    double highestAmplitude = -DBL_MAX;

    int i, tau;
    for (i = 0; i < max_positions_ptr; i++) {
        tau = max_positions[i];
        highestAmplitude = MAX(highestAmplitude, nsdf[tau]);

        if (nsdf[tau] > SMALL_CUTOFF) {
            parabolic_interpolation(tau);
            amp_estimates[amp_estimates_ptr++] = turning_point_y;
            period_estimates[period_estimates_ptr++] = turning_point_x;
            highestAmplitude = MAX(highestAmplitude, turning_point_y);
        }
    }

    if (period_estimates_ptr == 0) {
        pitch = -1;
    } else {
        double actualCutoff = CUTOFF * highestAmplitude;

        int periodIndex = 0;
        for (i = 0; i < amp_estimates_ptr; i++) {
            if (amp_estimates[i] >= actualCutoff) {
                periodIndex = i;
                break;
            }
        }

        double period = period_estimates[periodIndex];
        double pitchEstimate = (sampling_rate / period);
        if (pitchEstimate > LOWER_PITCH_CUTOFF) {
            pitch = pitchEstimate;
        } else {
            pitch = -1;
        }
    }
    return pitch;
}

void mpm::cleanup() {
    delete[] max_positions;
    delete[] period_estimates;
    delete[] amp_estimates;
}
