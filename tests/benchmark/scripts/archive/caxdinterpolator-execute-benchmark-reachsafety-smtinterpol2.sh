#!/bin/bash

CURRENT_DIR=$(pwd)
BENCHMARK_DIR=$(dirname $CURRENT_DIR)
OUTPUT_FILE=$BENCHMARK_DIR/benchmark_reachsafety_results-caxdinterpolator.txt
TESTS_DIR=$(dirname $BENCHMARK_DIR)
ROOT_DIR=$(dirname $TESTS_DIR)
# VERIFICATION_FILES_DIR=$(dirname $BENCHMARK_DIR)/verification-files/files
VERIFICATION_FILES_DIR=$ROOT_DIR/tests/smt2-files/benchmark/files

[ ! -f $OUTPUT_FILE ] && touch $OUTPUT_FILE
make -j8 -C $BENCHMARK_DIR $BENCHMARK_DIR/bin/benchmark

# for file in $VERIFICATION_FILES_DIR/ReachSafety-Arrays/array-fpi/pcompf.c_unreach-call.prp.smt2; do
#   pushd $BENCHMARK_DIR >> /dev/null
#   ./bin/benchmark $file 2 $OUTPUT_FILE 0
#   popd >> /dev/null
# done

# for file in $VERIFICATION_FILES_DIR/ReachSafety-Arrays/array-fpi/eqn1f.c_unreach-call.prp.smt2; do
#   pushd $BENCHMARK_DIR >> /dev/null
#   ./bin/benchmark $file 2 $OUTPUT_FILE 0
#   popd >> /dev/null
# done

for file in $VERIFICATION_FILES_DIR/ReachSafety-Arrays/array-fpi/eqn2f.c_unreach-call.prp.smt2; do
  pushd $BENCHMARK_DIR >> /dev/null
  ./bin/benchmark $file 2 $OUTPUT_FILE 0
  popd >> /dev/null
done

# for file in $VERIFICATION_FILES_DIR/ReachSafety-Arrays/array-fpi/eqn5f.c_unreach-call.prp.smt2; do
#   pushd $BENCHMARK_DIR >> /dev/null
#   ./bin/benchmark $file 2 $OUTPUT_FILE 0
#   popd >> /dev/null
# done

# for file in $VERIFICATION_FILES_DIR/ReachSafety-Arrays/array-fpi/*.smt2; do
#   pushd $BENCHMARK_DIR >> /dev/null
#   ./bin/benchmark $file 2 $OUTPUT_FILE 0
#   popd >> /dev/null
# done
