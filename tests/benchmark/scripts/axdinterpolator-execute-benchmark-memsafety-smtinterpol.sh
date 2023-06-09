#!/bin/bash

CURRENT_DIR=$(pwd)
BENCHMARK_DIR=$(dirname $CURRENT_DIR)
OUTPUT_FILE=$BENCHMARK_DIR/benchmark_memsafety_results-axdinterpolator.txt
# VERIFICATION_FILES_DIR=$(dirname $BENCHMARK_DIR)/verification-files/files
VERIFICATION_FILES_DIR=$BENCHMARK_DIR/files

[ ! -f $OUTPUT_FILE ] && touch $OUTPUT_FILE
make -j8 -C $BENCHMARK_DIR $BENCHMARK_DIR/bin/benchmark

for file in $VERIFICATION_FILES_DIR/MemSafety-Arrays/*/*.smt2; do
  pushd $BENCHMARK_DIR >> /dev/null
  ./bin/benchmark $file 2 $OUTPUT_FILE 0
  popd >> /dev/null
done
