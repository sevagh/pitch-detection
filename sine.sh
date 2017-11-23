#!/usr/bin/env bash

set -o errtrace
set -o nounset
set -o pipefail
set -o errexit

function usage() {
    printf "usage:\n\t%s <size> <freq in Hz> [<samplerate in Hz>]\n" "${0##*/}" >&2
    exit 1
}

function round() {
    echo $(printf %.$2f $(compute "scale=$2;(((10^$2)*$1)+0.5)/(10^$2)"))
}

function trunc() {
    echo ${1%%.*}
}

function compute() {
    bc -l <<< "${1}"
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

PI=3.14159265358979323846

if [ -z "${SIZE}" ] || [ -z "${FREQ}" ]; then
    usage
fi

declare -a LUT=() SINE=()

lut_size=$(trunc $(compute "${SIZE}"/2))
delta_phi=$(compute "${FREQ}*${lut_size}*1/${SAMPLERATE}")
phase=0.0

for ((i=0; i<lut_size; ++i)); do
    LUT+=($(round $(compute "32767 * s(2 * ${PI} * ${i}/${lut_size})")  0))
done

for ((i=0; i<SIZE; ++i)); do
    val=${LUT[$(trunc ${phase} 0)]}
    SINE+=(${val})
    phase=$(compute "${phase}+${delta_phi}")
    if [ "$(compute "${phase}>=${lut_size}")" -eq "1" ]; then
        phase=$(compute "${phase}-${lut_size}")
    fi
done

for i in ${SINE[@]}; do echo $i; done
