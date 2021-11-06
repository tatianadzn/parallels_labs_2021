#!/usr/bin/env bash
set -euxo pipefail

FILENAME=lab2
# FILENAME=lab2-verification

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"
mkdir -p $OUT_DIR

# Compile with Intel C Compiler
# icc -ipp -o $OUT_DIR/${FILENAME}-icc-seq $WORK_DIR/${FILENAME}.c
OMP_NUM_THREADS=1 icc -ipp -ipp-link=dynamic -parallel -qopenmp -o $OUT_DIR/${FILENAME}-icc-par-1 $WORK_DIR/${FILENAME}.c
OMP_NUM_THREADS=2 icc -ipp -ipp-link=dynamic -parallel -qopenmp -o $OUT_DIR/${FILENAME}-icc-par-2 $WORK_DIR/${FILENAME}.c
OMP_NUM_THREADS=3 icc -ipp -ipp-link=dynamic -parallel -qopenmp -o $OUT_DIR/${FILENAME}-icc-par-3 $WORK_DIR/${FILENAME}.c
OMP_NUM_THREADS=4 icc -ipp -ipp-link=dynamic -parallel -qopenmp -o $OUT_DIR/${FILENAME}-icc-par-4 $WORK_DIR/${FILENAME}.c
OMP_NUM_THREADS=6 icc -ipp -ipp-link=dynamic -parallel -qopenmp -o $OUT_DIR/${FILENAME}-icc-par-6 $WORK_DIR/${FILENAME}.c
OMP_NUM_THREADS=8 icc -ipp -ipp-link=dynamic -parallel -qopenmp -o $OUT_DIR/${FILENAME}-icc-par-8 $WORK_DIR/${FILENAME}.c