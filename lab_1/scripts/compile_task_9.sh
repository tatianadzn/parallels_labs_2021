#!/usr/bin/env bash
set -euxo pipefail

gcc -O3 -Wall -Werror -o ../lab1-verification-seq ../lab1_verification.c -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=1 ../lab1_verification.c -o ../lab1-verification-par-1 -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=2 ../lab1_verification.c -o ../lab1-verification-par-2 -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=3 ../lab1_verification.c -o ../lab1-verification-par-3 -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=4 ../lab1_verification.c -o ../lab1-verification-par-4 -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=6 ../lab1_verification.c -o ../lab1-verification-par-6 -lm
gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=8 ../lab1_verification.c -o ../lab1-verification-par-8 -lm