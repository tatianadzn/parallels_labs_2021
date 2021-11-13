#!/usr/bin/env bash

FILENAME_SEQ=lab3-wo-openmp
FILENAME=lab3-openmp

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

##run without defining schedule type
#echo "default schedule"
#echo "N,k(N)"
#for i in {0..10}
#do
#  let N=delta*i+N1
#  $OUT_DIR/${FILENAME} $N $Num_threads 0 0
#done
#

##run with defining schedule type - static and chunk sizes 1, 2, 4, 6
#echo "static schedule"
#echo "N,k(N)"
#for i in {0..10}
#do
#  for chunk_size in {1,2,4,6}
#  do
#    let N=delta*i+N1
#    $OUT_DIR/${FILENAME} $N $Num_threads 1 chunk_size
#  done
#done
#
##run with defining schedule type - dynamic and chunk sizes 1, 2, 4, 6
#echo "dynamic schedule"
#echo "N,k(N)"
#for i in {0..10}
#do
#  for chunk_size in {1,2,4,6}
#  do
#    let N=delta*i+N1
#    $OUT_DIR/${FILENAME} $N $Num_threads 2 chunk_size
#  done
#done
#
##run with defining schedule type - guided and chunk sizes 1, 2, 4, 6
#echo "guided schedule"
#echo "N,k(N)"
#for i in {0..10}
#do
#  for chunk_size in {1,2,4,6}
#  do
#    let N=delta*i+N1
#    $OUT_DIR/${FILENAME} $N $Num_threads 3 chunk_size
#  done
#done
