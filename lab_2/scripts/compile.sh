#!/usr/bin/env bash
set -euxo pipefail

FILENAME1=lab2
FILENAME2=lab2-verification

WORK_DIR=".."
OUT_DIR="$WORK_DIR/bin"
mkdir -p $OUT_DIR

# Compile
gcc -O3 -Wall -Werror -I${FW_HOME} -L${FW_LIB} -o $OUT_DIR/${FILENAME1}-fw $WORK_DIR/${FILENAME1}.c -lfwBase -lfwSignal -lm
gcc -O3 -Wall -Werror -I${FW_HOME} -L${FW_LIB} -o $OUT_DIR/${FILENAME2}-fw $WORK_DIR/${FILENAME2}.c -lfwBase -lfwSignal -lm