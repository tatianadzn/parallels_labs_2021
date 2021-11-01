#!/usr/bin/env bash

FILENAME=lab3

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"
mkdir -p $OUT_DIR

# Compile with Gnu C Compiler
gcc -O3 -Wall -Werror -fopenmp -o $OUT_DIR/${FILENAME} $WORK_DIR/${FILENAME}.c -lm


N1=2300
N2=780000

### change for different experiments
Num_threads = 2

let delta=(N2-N1)/10

#run without defining schedule type
echo "default schedule"
echo "N,k(N)"
for i in {0..10}
do
  let N=delta*i+N1
  $OUT_DIR/${FILENAME} $N $Num_threads 0 0
done


#run with defining schedule type - static and chunk sizes 1, 2, 4, 6
echo "static schedule"
echo "N,k(N)"
for i in {0..10}
do
  for chunk_size in {1,2,4,6}
  do
    let N=delta*i+N1
    $OUT_DIR/${FILENAME} $N $Num_threads 1 chunk_size
  done
done

#run with defining schedule type - dynamic and chunk sizes 1, 2, 4, 6
echo "dynamic schedule"
echo "N,k(N)"
for i in {0..10}
do
  for chunk_size in {1,2,4,6}
  do
    let N=delta*i+N1
    $OUT_DIR/${FILENAME} $N $Num_threads 2 chunk_size
  done
done

#run with defining schedule type - guided and chunk sizes 1, 2, 4, 6
echo "guided schedule"
echo "N,k(N)"
for i in {0..10}
do
  for chunk_size in {1,2,4,6}
  do
    let N=delta*i+N1
    $OUT_DIR/${FILENAME} $N $Num_threads 3 chunk_size
  done
done
