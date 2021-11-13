#!/bin/bash

FILENAME=lab2
# FILENAME=lab2-verification

# Yunir
N1=1300
N2=600000

let delta=(N2-N1)/10

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"

# 8.2 K=1
echo "K = 1"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-fw $N 1
done

# 8.3 K=2
echo "K = 2"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-fw $N 2
done

# 8.4 K=3
echo "K = 3"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-fw $N 3
done

# 8.5 K=4
echo "K = 4"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-fw $N 4
done

# 8.6 K=6
echo "K = 6"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-fw $N 6
done

# 8.7 K=8
echo "K = 8"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME}-fw $N 8
done