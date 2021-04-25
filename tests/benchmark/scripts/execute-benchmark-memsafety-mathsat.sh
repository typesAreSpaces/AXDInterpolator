#!/bin/bash

VERIFICATION_FILES_DIR=/media/Documents/MaxDiff-Experiments/verification-files
CURRENT_DIR=$(pwd)
BENCHMARK_DIR=$(dirname $CURRENT_DIR)
OUTPUT_FILE=$BENCHMARK_DIR/benchmark_memsafety_results.txt

[ ! -f $OUTPUT_FILE] && touch $OUTPUT_FILE
make -j8 -C $BENCHMARK_DIR bin/benchmark

for file in $VERIFICATION_FILES_DIR/MemSafety-Arrays/*/*.smt2; do
  pushd $BENCHMARK_DIR >> /dev/null
  ./bin/benchmark $file 1 $OUTPUT_FILE
  popd >> /dev/null
done
