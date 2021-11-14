#!/usr/bin/env bash
set -euxo pipefail

FILENAME=lab3
FILENAME_SCHEDULE=lab3_schedule

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"
mkdir -p $OUT_DIR

# Compile
gcc -O3 -Wall -Werror -Wno-unknown-pragmas -o $OUT_DIR/${FILENAME}-wo-openmp $WORK_DIR/${FILENAME}.c -lm
gcc -O3 -Wall -Werror -fopenmp -o $OUT_DIR/${FILENAME}-openmp $WORK_DIR/${FILENAME}.c -lm

# static
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=static -DCHUNCK_SIZE=1 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-static-1 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=static -DCHUNCK_SIZE=2 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-static-2 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=static -DCHUNCK_SIZE=4 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-static-4 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=static -DCHUNCK_SIZE=8 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-static-8 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm

# dynamic
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=dynamic -DCHUNCK_SIZE=1 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-dynamic-1 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=dynamic -DCHUNCK_SIZE=2 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-dynamic-2 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=dynamic -DCHUNCK_SIZE=4 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-dynamic-4 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=dynamic -DCHUNCK_SIZE=8 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-dynamic-8 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm

# guided
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=guided -DCHUNCK_SIZE=1 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-guided-1 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=guided -DCHUNCK_SIZE=2 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-guided-2 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=guided -DCHUNCK_SIZE=4 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-guided-4 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm
gcc -O3 -Wall -Werror -fopenmp -DSCHEDULE_TYPE=guided -DCHUNCK_SIZE=8 -o $OUT_DIR/${FILENAME_SCHEDULE}-openmp-guided-8 $WORK_DIR/${FILENAME_SCHEDULE}.c -lm