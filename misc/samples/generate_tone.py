#!/usr/bin/env python3

import numpy as np
import librosa
import sys


def generate_tone(frequency, duration_samples, sample_rate):
    # Generate a sine wave tone
    t = np.linspace(0, duration_samples / sample_rate, duration_samples, endpoint=False)
    tone = np.sin(2 * np.pi * frequency * t)

    # Apply a window function (e.g., Hanning) to smooth the start and end of the tone
    window = np.hanning(duration_samples)
    windowed_tone = tone * window

    return windowed_tone


if __name__ == '__main__':
    try:
        frequency = float(sys.argv[1])
        duration_samples = int(sys.argv[2])
        sample_rate = int(sys.argv[3])
        outputfile = sys.argv[4]
    except IndexError:
        print('usage: generate_tone.py frequency duration_samples sample_rate outfile', file=sys.stderr)
        sys.exit(1)

    tone = generate_tone(frequency, duration_samples, sample_rate)

    with open(outputfile, 'w') as fwrite:
        for sample in tone:
            fwrite.write('{0}\n'.format(sample))

    # Pitch detection for sanity check
    f0 = librosa.yin(y=tone, sr=sample_rate, fmin=65, fmax=2093, frame_length=8192, hop_length=8192)
    print("Estimated Pitch:", f0)
