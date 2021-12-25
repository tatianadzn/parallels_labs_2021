#!/usr/bin/env bash
set -euxo pipefail

FILENAME=lab5

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"
mkdir -p $OUT_DIR

# Compile
gcc -O3 -Wall -Werror -Wno-unknown-pragmas -o $OUT_DIR/${FILENAME} -pthread $WORK_DIR/${FILENAME}.c -lm
