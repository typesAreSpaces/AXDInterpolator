#!/bin/bash

VERIFICATION_FILES_DIR=/media/Documents/MaxDiff-Experiments/verification-files
CURRENT_DIR=$(pwd)
BENCHMARK_DIR=$(dirname $CURRENT_DIR)
OUTPUT_FILE=$BENCHMARK_DIR/benchmark_reachsafety_results-z3.txt

[ ! -f $OUTPUT_FILE ] && touch $OUTPUT_FILE
make -j8 -C $BENCHMARK_DIR bin/benchmark

for file in $VERIFICATION_FILES_DIR/ReachSafety-Arrays/*/*.smt2; do
  pushd $BENCHMARK_DIR >> /dev/null
  ./bin/benchmark $file 0 $OUTPUT_FILE 1
  popd >> /dev/null
done
