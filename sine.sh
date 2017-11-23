#!/usr/bin/env bash

set -o errtrace
set -o nounset
set -o pipefail
set -o errexit

CC="${CC:-gcc}"

function usage() {
    printf "usage:\n\t%s <size> <freq in Hz> [<samplerate in Hz>]\n" "${0##*/}" >&2
    exit 1
}

while getopts ":h" opt; do
    case "$opt" in
        h) usage;;
        \?)
            echo "$0: unrecognized flag: -$OPTARG. run with '-h' for help"
            exit 1 ;;
        :)
            echo "$0: flag -$OPTARG requires an argument"
            exit 1 ;;
    esac
done

shift $((OPTIND - 1))
SIZE=${1:-}
FREQ=${2:-}
SAMPLERATE=${3:-48000}

EXECUTABLE_LOCATION="/tmp/sinewave_exec_$(date -u +%s)"

if [ -z "${SIZE}" ] || [ -z "${FREQ}" ]; then
    usage
fi

echo "#include <stdio.h>
#include <math.h>

#define M_PI 3.14159265358979323846

void generate_sinewave(double *sinewave, int size, double frequency, int sample_rate)
{
        int lut_size = size/4;

        int lut[lut_size];

        double doublef = (double) frequency;
        double delta_phi = doublef * lut_size * 1.0 / sample_rate;
        double phase = 0.0;

        for (int i = 0; i < lut_size; ++i)
                lut[i] = (int) roundf(0x7FFF * sinf(2.0 * M_PI * i / lut_size));

        for (int i = 0; i < size/2; ++i) {
                int val = lut[(int) phase];
                sinewave[i] = val;
                phase += delta_phi;
                if (phase >= lut_size) phase -= lut_size;
        }
}

int main() {
        int size = "${SIZE}";
        double sinewave[size];
        generate_sinewave(sinewave, size, "${FREQ}", 48000);
        for (int i = 0; i < size; ++i)
                printf(\"%f\\n\", sinewave[i]);
        return 0;
}" | "${CC}" -x c -o "${EXECUTABLE_LOCATION}" -lm -

"${EXECUTABLE_LOCATION}"

rm "${EXECUTABLE_LOCATION}"
