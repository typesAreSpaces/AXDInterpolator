#!/usr/bin/env bash

FILE=../smt2-files/benchmark/files/MemSafety-Arrays/array-examples/relax-2.c_valid-memsafety.prp.smt2 

rm -rf debug_log.txt
touch debug_log.txt

# caxdinterpolator(z3)
./bin/benchmark $FILE 0 debug_log.txt 0
# axdinterpolator(z3)
./bin/benchmark $FILE 0 debug_log.txt 1
# other_solvers(z3)
./bin/benchmark $FILE 0 debug_log.txt 2

# caxdinterpolator(mathsat)
./bin/benchmark $FILE 1 debug_log.txt 0
# axdinterpolator(mathsat)
./bin/benchmark $FILE 1 debug_log.txt 1
# other_solvers(mathsat)
./bin/benchmark $FILE 1 debug_log.txt 2

# caxdinterpolator(smtinterpol)
./bin/benchmark $FILE 2 debug_log.txt 0
# axdinterpolator(smtinterpol)
./bin/benchmark $FILE 2 debug_log.txt 1
# other_solvers(smtinterpol)
./bin/benchmark $FILE 2 debug_log.txt 2
