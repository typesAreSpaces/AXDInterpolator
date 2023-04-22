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

THEORY=QF_TO
# THEORY=QF_LIA

TEST_DIR=tests/smt2-files
# CURR_TEST=6_8.smt2
# CURR_TEST=7_1.smt2
# CURR_TEST=7_2.smt2
CURR_TEST=not_7_2.smt2
ALLOWED_ATTEMPTS=100

echo ">> Checking file" "$TEST_DIR/$CURR_TEST"

./bin/axd_interpolator \
    $THEORY $TEST_DIR/$CURR_TEST \
    $INTERPOLATION_ENGINE $ALLOWED_ATTEMPTS
