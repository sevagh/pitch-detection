#!/usr/bin/env python3

import os
import sys
import subprocess
import random


if __name__ == '__main__': 
    hyperfine = subprocess.Popen('hyperfine',
            shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)
    out, err = hyperfine.communicate()
    if hyperfine.returncode != 0 and err and 'The following required arguments were not provided' not in str(err):
        print('Hyperfine problems: {0}'.format(err), file=sys.stderr)
        sys.exit(hyperfine.returncode)

    pitches = [
            83.3,
            1337.0,
            537.0,
            699.9,
            343.2,
            2134.0,
            1234.0,
            555.5,
            ]

    sizes = [
            64000,
            64303,
            ]

    algo = None
    try:
        algo = sys.argv[1] 
    except IndexError:
        algo = 'mpm'

    for size in sizes:
        for pitch in pitches:
            sinewave_cmd = './bin/sinewave {0} {1} {2} 48000'.format(pitch, algo, size)
            qrun = subprocess.Popen(
                    sinewave_cmd,
                    shell=True,
                    stdout=subprocess.PIPE).communicate()[0]
            _, expect, got = qrun[:-1].decode('utf-8').split('\t')
            expect = float(expect.split()[-1])
            got = float(got.split()[-1])
            print('Expect: {0:.2f}, Got: {1:.2f}, Error: {2:.2f}'.format(expect, got, (abs(expect - got)/expect)*100.0))
            brun = subprocess.Popen(
                    "hyperfine --min-runs 10 --warmup 3 '{0}'".format(sinewave_cmd),
                    shell=True).communicate()
