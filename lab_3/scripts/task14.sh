#!/usr/bin/env bash

FILENAME=lab3_schedule-openmp

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"
mkdir -p $OUT_DIR

N1=1300
N2=600000

let delta=(N2-N1)/10

schedule_type="guided"
chunk_size=1
Num_threads=4
for optimize in {0..3}
do
  echo "optimize = ${optimize}"
  echo "N,k(N)"
  for i in {0..10}
  do
    let N=delta*i+N1
    $OUT_DIR/${FILENAME}-${schedule_type}-${chunk_size}-optimize-o${optimize} $N $Num_threads
  done
done
