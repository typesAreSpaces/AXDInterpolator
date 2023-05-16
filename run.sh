#!/usr/bin/env bash

if [[ "$OSTYPE" == "darwin"* ]]; then
    LDIR=lib
    DYLD_LIBRARY_PATH=$LDIR
    export DYLD_LIBRARY_PATH
fi

iZ3=0
MathSAT=1
SMTInterpolator=2
INTERPOLATION_ENGINE=$iZ3

THEORY=QF_LIA

TEST_DIR=tests/smt2-files/caxd_examples
# CURR_TEST=6_8.smt2
# CURR_TEST=7_1.smt2
CURR_TEST=7_2.smt2
# CURR_TEST=not_7_2.smt2
# CURR_TEST=jhala.smt2
# CURR_TEST=modified_jhala.smt2

echo ">> Checking file" "$TEST_DIR/$CURR_TEST"

./bin/caxd_interpolator \
    $THEORY $TEST_DIR/$CURR_TEST \
    $INTERPOLATION_ENGINE
