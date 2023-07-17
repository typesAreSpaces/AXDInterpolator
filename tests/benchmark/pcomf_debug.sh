#!/usr/bin/env bash

ulimit -St 360 -Sv 4500000

./bin/benchmark ../../tests/smt2-files/benchmark/files/ReachSafety-Arrays/array-fpi/pcompf.c_unreach-call.prp.smt2 2 here.txt 0  

ulimit -St unlimited -Sv unlimited
