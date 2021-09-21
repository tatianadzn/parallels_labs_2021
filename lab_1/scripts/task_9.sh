#!/bin/bash

# Tatiana
N1=1050
N2=372000

# Yunir
#N1=1300
#N2=600000

let delta=(N2-N1)/10

# 8.1
echo "N,seq(N)"
for i in {0..10}
do
  let N=delta*i+N1

done

# 8.2 K=1
echo "K = 1"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-verification-par-1 $N
done

# 8.3 K=2
echo "K = 2"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-verification-par-2 $N
done

# 8.3 K=4
echo "K = 4"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-verification-par-4 $N
done

# 8.4 K=6
echo "K = 6"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-verification-par-6 $N
done

# 8.5 K=8
echo "K = 8"
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-verification-par-8 $N
done

# 8.6 K=3
echo "K = 3"v
echo "N,K(N)"
for i in {0..10}
do
  let N=delta*i+N1
   ../lab1-verification-par-3 $N
done