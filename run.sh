#!/usr/bin/env bash

if [[ "$OSTYPE" == "darwin"* ]]; then
    LDIR=lib
    DYLD_LIBRARY_PATH=$LDIR
    export DYLD_LIBRARY_PATH
fi

iZ3=0
MathSAT=1
SMTInterpolator=2
INTERPOLATION_ENGINE=2

THEORY=QF_LIA

TEST_DIR=tests/smt2-files/caxd_examples
#CURR_TEST=6_8.smt2
#CURR_TEST=7_1.smt2
#CURR_TEST=7_2.smt2
#CURR_TEST=not_7_2.smt2
CURR_TEST=jhala.smt2
#CURR_TEST=modified_jhala.smt2

#TEST_DIR=tests/smt2-files/buggy-benchmark
#CURR_TEST=axdinterpolator_Z3_buggy2-simplified-2.smt2
#CURR_TEST=axdinterpolator_Z3_brs4f.c_unreach-call.prp.smt2 

#TEST_DIR=tests/smt2-files/buggy-benchmark2
#CURR_TEST=buggy-post-benchmark.smt2

#TEST_DIR=tests/smt2-files/problematic_files_caxdinterpolator_smtinterpol/check
#CURR_TEST=debug_this_156.smt2

#TEST_DIR=/home/jose
#CURR_TEST=debug_this_241.smt2
#CURR_TEST=debug_this_497.smt2

#CURR_TEST=debug_this_226.smt2
#CURR_TEST=debug_this_228.smt2
#CURR_TEST=debug_this_229.smt2
#CURR_TEST=debug_this_230.smt2
#CURR_TEST=debug_this_231.smt2
#CURR_TEST=debug_this_272.smt2
#CURR_TEST=debug_this_323.smt2
#CURR_TEST=debug_this_361.smt2
#CURR_TEST=debug_this_363.smt2
#CURR_TEST=debug_this_446.smt2
#CURR_TEST=debug_this_448.smt2

echo ">> Checking file" "$TEST_DIR/$CURR_TEST"

./bin/caxd_interpolator \
    $THEORY $TEST_DIR/$CURR_TEST \
    $INTERPOLATION_ENGINE

# for x in $TEST_DIR/*.smt2; do
#    echo $x
#    ./bin/caxd_interpolator \
    #      $THEORY $x \
    #      $INTERPOLATION_ENGINE
#       done;
