#!/usr/bin/env bash

FILENAME=lab3_schedule-openmp

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"
mkdir -p $OUT_DIR

N1=1300
N2=600000

let delta=(N2-N1)/10

for schedule_type in {"static","dynamic","guided"}
do
  for chunk_size in {1,2,4,8}
  do
    for Num_threads in {1,2,3,4,6,8}
    do
      echo "schedule_type = $schedule_type chunk_size = $chunk_size K = $Num_threads"
      echo "N,k(N)"
      for i in {0..10}
      do
        let N=delta*i+N1
        $OUT_DIR/${FILENAME}-${schedule_type}-${chunk_size} $N $Num_threads
      done
    done
    sleep 5
  done
done