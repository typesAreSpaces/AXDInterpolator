#!/usr/bin/env bash

FILE=../smt2-files/caxd_examples/7_2.smt2

rm -rf debug_log.txt
touch debug_log.txt

gdb --args ./bin/benchmark $FILE 0 debug_log.txt 0
