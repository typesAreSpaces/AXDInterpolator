#!/usr/bin/env bash

if [[ "$OSTYPE" == "darwin"* ]]; then
    LDIR=lib
    DYLD_LIBRARY_PATH=$LDIR
    export DYLD_LIBRARY_PATH
fi

THEORY=QF_LIA
TEST_DIR=tests/smt2-files/caxd_examples/
CURR_TEST=${TEST_DIR}/6_2.smt2

./bin/axd_interpolator \
    $THEORY $CURR_TEST \
    0 1000000
