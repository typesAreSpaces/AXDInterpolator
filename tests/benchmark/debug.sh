#!/usr/bin/env bash

rm -rf debug_log.txt
#./bin/benchmark buggy.smt2 0 debug_log.txt 0
#./bin/benchmark buggy2.smt2 0 debug_log.txt 0
./bin/benchmark buggy2-simplified.smt2 0 debug_log.txt 0
#./bin/benchmark buggy2-simplified-but-works-this-time.smt2 0 debug_log.txt 0

#gdb --args ./bin/benchmark buggy2-simplified.smt2 0 debug_log.txt 0
