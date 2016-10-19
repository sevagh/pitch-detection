#!/bin/bash

cd pitch_detection/ && cmake . && make && ./pitch_detection && ./clean.sh
