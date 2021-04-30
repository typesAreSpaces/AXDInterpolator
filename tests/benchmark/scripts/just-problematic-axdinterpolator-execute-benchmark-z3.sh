#!/bin/bash

VERIFICATION_FILES_DIR=/home/jose/Documents/GithubProjects/AXDInterpolator/tests/problematic_files
CURRENT_DIR=$(pwd)
BENCHMARK_DIR=$(dirname $CURRENT_DIR)
OUTPUT_FILE=$BENCHMARK_DIR/benchmark_problematic_results-axdinterpolator.txt

[ ! -f $OUTPUT_FILE ] && touch $OUTPUT_FILE
make -j8 -C $BENCHMARK_DIR bin/benchmark

for file in $VERIFICATION_FILES_DIR/*.smt2*; do
  pushd $BENCHMARK_DIR >> /dev/null
  ./bin/benchmark $file 0 $OUTPUT_FILE 0
  popd >> /dev/null
done
