#!/usr/bin/env bash

CURRENT_DIR=$(pwd)
LDIR=$CURRENT_DIR/lib
DYLD_LIBRARY_PATH=$LDIR
export DYLD_LIBRARY_PATH

$CURRENT_DIR/bin/axd_interpolator \
  QF_LIA \
  $CURRENT_DIR/tests/smt2-files/strcpy_example_variant_1.smt2 \
  0 1000000
