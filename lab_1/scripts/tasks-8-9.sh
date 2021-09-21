#!/bin/bash

# Tasks 8
FILENAME=lab1

# Task 9
# FILENAME=lab1-verification

# Yunir
N1=1300
N2=600000
COMPILER="gcc"

let delta=(N2-N1)/10

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"

# 8.1
echo "N,seq(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-$COMPILER-seq $N
done

# 8.2 K=1
echo "K = 1"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-$COMPILER-par-1 $N
done

# 8.3 K=2
echo "K = 2"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-$COMPILER-par-2 $N
done

# 8.4 K=3
echo "K = 3"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-$COMPILER-par-3 $N
done

# 8.5 K=4
echo "K = 4"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-$COMPILER-par-4 $N
done

# 8.6 K=6
echo "K = 6"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-$COMPILER-par-6 $N
done

# 8.7 K=8
echo "K = 8"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-$COMPILER-par-8 $N
done