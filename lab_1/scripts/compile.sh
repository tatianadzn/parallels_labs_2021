#!/usr/bin/env bash
set -euxo pipefail

# Tasks 3-8
FILENAME=lab1

# Task 9
# FILENAME=lab1-verification

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"
mkdir -p $OUT_DIR

# Compile with Gnu C Compiler
gcc -O3 -Wall -Werror -o $OUT_DIR/${FILENAME}-gcc-seq $WORK_DIR/${FILENAME}.c -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=1 $WORK_DIR/${FILENAME}.c -o $OUT_DIR/${FILENAME}-gcc-par-1 -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=2 $WORK_DIR/${FILENAME}.c -o $OUT_DIR/${FILENAME}-gcc-par-2 -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=3 $WORK_DIR/${FILENAME}.c -o $OUT_DIR/${FILENAME}-gcc-par-3 -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=4 $WORK_DIR/${FILENAME}.c -o $OUT_DIR/${FILENAME}-gcc-par-4 -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=6 $WORK_DIR/${FILENAME}.c -o $OUT_DIR/${FILENAME}-gcc-par-6 -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=8 $WORK_DIR/${FILENAME}.c -o $OUT_DIR/${FILENAME}-gcc-par-8 -lm

# Compile with Intel C Compiler
icc -o $OUT_DIR/${FILENAME}-icc-seq $WORK_DIR/${FILENAME}.c
icc -parallel -par-threshold=0 -par-num-threads=1 -o $OUT_DIR/${FILENAME}-icc-par-1 $WORK_DIR/${FILENAME}.c
icc -parallel -par-threshold=0 -par-num-threads=2 -o $OUT_DIR/${FILENAME}-icc-par-2 $WORK_DIR/${FILENAME}.c
icc -parallel -par-threshold=0 -par-num-threads=3 -o $OUT_DIR/${FILENAME}-icc-par-3 $WORK_DIR/${FILENAME}.c
icc -parallel -par-threshold=0 -par-num-threads=4 -o $OUT_DIR/${FILENAME}-icc-par-4 $WORK_DIR/${FILENAME}.c
icc -parallel -par-threshold=0 -par-num-threads=6 -o $OUT_DIR/${FILENAME}-icc-par-6 $WORK_DIR/${FILENAME}.c
icc -parallel -par-threshold=0 -par-num-threads=8 -o $OUT_DIR/${FILENAME}-icc-par-8 $WORK_DIR/${FILENAME}.c