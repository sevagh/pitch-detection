#!/usr/bin/env bash
# This file:
#
#  - Entrypoint for pitch-detection Docker test image 
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

srcdir="/pitch-detection/"
workdir="/pitch-detection-wrk"

# main
cp -r "$srcdir" "$workdir"
if [ $# -ne 0 ]; then
    apt install -y "$@" >/dev/null 2>&1
fi
cd "$workdir"
cmake . >/dev/null 2>&1
make >/dev/null 2>&1
./pitch_detection version
./housekeeping.sh clean
