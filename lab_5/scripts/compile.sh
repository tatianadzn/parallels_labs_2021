#!/usr/bin/env bash
set -euxo pipefail

FILENAME=lab5

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"
mkdir -p $OUT_DIR

# Compile
gcc -O3 -Wall -Werror -Wno-unknown-pragmas -o $OUT_DIR/${FILENAME}-wo-openmp $WORK_DIR/${FILENAME}.c -lm
gcc -O3 -Wall -Werror -fopenmp -o $OUT_DIR/${FILENAME}-openmp $WORK_DIR/${FILENAME}.c -lm