#!/usr/bin/env bash

OUTPUT_FILE=debugging_pcompf.c_unreach-call.prp.txt

ulimit -St 360 -Sv 4500000

[ ! -f $OUTPUT_FILE ] && touch $OUTPUT_FILE
./../bin/benchmark ../../../tests/smt2-files/benchmark/files/ReachSafety-Arrays/array-fpi/pcompf.c_unreach-call.prp.smt2 2 $OUTPUT_FILE 0  

ulimit -St unlimited -Sv unlimited
