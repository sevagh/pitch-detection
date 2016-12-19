#!/usr/bin/env bash
# This file:
#
#  - Docker/bash test script
#
# Usage:
#
#  LOG_LEVEL=7 ./main.sh -f /tmp/x -d (change this for your script)
#
# Based on a template by BASH3 Boilerplate v2.0.0
# Copyright (c) 2013 Kevin van Zonneveld and contributors
# http://bash3boilerplate.sh/#authors

set -o errexit
set -o errtrace
set -o nounset
set -o pipefail

docker_img_name="github.com/sevagh/pitch-detection"
docker_dir="./docker"
dockerfile="./docker/Dockerfile"

# functions
function docker_build_run() {
    echo 'Building container'
    docker build -q -t "$docker_img_name" "$docker_dir" -f "$dockerfile"
    echo "$@"
    echo 'Running container'
    docker run --privileged \
        -v "$PWD/../":/pitch-detection \
        --rm -it \
        "$docker_img_name" \
	"$@"
}

function quality_checker() {
    cppcheck ./src --force
    cppclean --include-path=./src ./
}

function docker_build_runner() {
    echo "Testing build without ffmpeg or fftw"
    docker_build_run
    echo "Testing build with fftw"
    docker_build_run libfftw3-dev
    echo "Testing build with ffmpeg"
    docker_build_run libavcodec-dev libavformat-dev libavutil-dev
    echo "Testing build with ffmpeg and fftw"
    docker_build_run libavcodec-dev libavformat-dev libavutil-dev libfftw3-dev
}

function usage() {
    printf "usage: $0 <clean/quality/build>\n"
    exit -1
}

# main
if [ $# -ne 0 ]; then
    if [ z"$1" = z"build" ]; then
        docker_build_runner
    elif [ z"$1" = z"quality" ]; then
        quality_checker
    else
	usage
    fi
else
    usage
fi
