#!/usr/bin/env python3

import numpy as np
import librosa
import random
import sys


if __name__ == '__main__':
    try:
        frequency = float(sys.argv[1])
        duration_samples = int(sys.argv[2])
        sample_rate = int(sys.argv[3])
        outputfile = sys.argv[4]
    except IndexError:
        print('usage: generate_tone.py frequency duration_samples sample_rate outfile', file=sys.stderr)
        sys.exit(1)

    extended_duration_samples = 2 * duration_samples
    extended_tone = librosa.tone(frequency, sr=sample_rate, length=extended_duration_samples)

    start = random.randint(0, duration_samples)
    tone = extended_tone[start:start + duration_samples]

    with open(outputfile, 'w') as fwrite:
        for sample in tone:
            fwrite.write('{0}\n'.format(sample))

    # Pitch detection for sanity check
    f0 = librosa.yin(y=tone, sr=sample_rate, fmin=65, fmax=2093, frame_length=duration_samples)
    print("Estimated Pitch:", f0)
