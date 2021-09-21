#!/bin/bash

# Tatiana
N1=1050
N2=372000

# Yunir
#N1=1300
#N2=600000

let delta=(N2-N1)/10

# 8.1
echo "N,seq(N)" > test.csv
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-seq $N >> test.csv
done

# 8.2 K=1
echo "K = 1" >> test.csv
echo "N,K(N)" >> test.csv
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-par-1 $N >> test.csv
done

# 8.3 K=2
echo "K = 2" >> test.csv
echo "N,K(N)" >> test.csv
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-par-2 $N >> test.csv
done

# 8.3 K=4
echo "K = 4" >> test.csv
echo "N,K(N)" >> test.csv
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-par-4 $N >> test.csv
done

# 8.4 K=6
echo "K = 6" >> test.csv
echo "N,K(N)" >> test.csv
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-par-6 $N >> test.csv
done

# 8.5 K=8
echo "K = 8" >> test.csv
echo "N,K(N)" >> test.csv
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-par-8 $N >> test.csv
done