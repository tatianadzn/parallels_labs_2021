#!/usr/bin/env bash

FILENAME_SEQ=lab4-task7.1-wo-openmp
FILENAME=lab4-task7.1-openmp

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"
mkdir -p $OUT_DIR

N1=1300
N2=600000

let delta=(N2-N1)/10

echo "N,seq(N)"
for i in {0..10}
do
  let N=delta*i+N1
$OUT_DIR/${FILENAME_SEQ} $N 0
done

#run without defining schedule type
for Num_threads in {1,2,3,4,6,8}
do
  echo "K = $Num_threads"
  echo "N,k(N)"
  for i in {0..10}
  do
    let N=delta*i+N1
    $OUT_DIR/${FILENAME} $N $Num_threads
  done
done
