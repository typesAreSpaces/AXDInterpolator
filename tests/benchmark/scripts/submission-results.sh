#!/bin/bash

CURRENT_DIR=$(pwd)
BENCHMARK_DIR=$(dirname $CURRENT_DIR)
TESTS_DIR=$(dirname $BENCHMARK_DIR)
ROOT_DIR=$(dirname $TESTS_DIR)
BENCHMARK_FILES_DIR=$ROOT_DIR/tests/smt2-files/benchmark
VERIFICATION_FILES_DIR=$BENCHMARK_FILES_DIR/files

# Post-processing script
$CURRENT_DIR/process-results/submission-results.py $VERIFICATION_FILES_DIR $BENCHMARK_DIR

make -C $BENCHMARK_DIR clean

ulimit -St unlimited -Sv unlimited
