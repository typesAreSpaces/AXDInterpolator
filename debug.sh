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

#TEST_DIR=tests/smt2-files/caxd_examples
#CURR_TEST=7_2_2.smt2

TEST_DIR=tests/smt2-files/buggy-benchmark
#CURR_TEST=axdinterpolator_Z3_buggy2-simplified-2.smt2
CURR_TEST=axdinterpolator_Z3_brs4f.c_unreach-call.prp.smt2 

echo ">> Debugging file" "$TEST_DIR/$CURR_TEST"

gdb --args ./bin/caxd_interpolator \
    $THEORY $TEST_DIR/$CURR_TEST \
    $INTERPOLATION_ENGINE

