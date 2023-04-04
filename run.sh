#!/usr/bin/env bash

if [[ "$OSTYPE" == "darwin"* ]]; then
    LDIR=lib
    DYLD_LIBRARY_PATH=$LDIR
    export DYLD_LIBRARY_PATH
fi

./bin/axd_interpolator \
    QF_LIA \
    tests/smt2-files/strcpy_example_variant_1.smt2 \
    0 1000000
