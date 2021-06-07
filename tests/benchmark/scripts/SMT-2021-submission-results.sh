#!/bin/bash

make -C ../ -j8 bin/benchmark

CURRENT_DIR=$(pwd)
BENCHMARK_DIR=$(dirname $CURRENT_DIR)
TESTS_DIR=$(dirname $BENCHMARK_DIR)

ulimit -St 360 -Sv 4500000

echo "How many cores can be used? 1, 3, or 6"
read num_of_cores_allowed

if [ "$num_of_cores_allowed" == "1" ]; then
  ./axdinterpolator-execute-benchmark-memsafety-z3.sh
  ./axdinterpolator-execute-benchmark-memsafety-mathsat.sh
  ./axdinterpolator-execute-benchmark-memsafety-smtinterpol.sh
  ./axdinterpolator-execute-benchmark-reachsafety-z3.sh
  ./axdinterpolator-execute-benchmark-reachsafety-mathsat.sh
  ./axdinterpolator-execute-benchmark-reachsafety-smtinterpol.sh

  cat $BENCHMARK_DIR/benchmark_memsafety_results-* > $BENCHMARK_DIR/benchmark_memsafety_results_axdinterpolator.txt
  cat $BENCHMARK_DIR/benchmark_reachsafety_results-* > $BENCHMARK_DIR/benchmark_reachsafety_results_axdinterpolator.txt
  rm -rf $BENCHMARK_DIR/benchmark_memsafety_results-*
  rm -rf $BENCHMARK_DIR/benchmark_reachsafety_results-*

  ./execute-benchmark-memsafety-z3.sh
  ./execute-benchmark-memsafety-mathsat.sh
  ./execute-benchmark-memsafety-smtinterpol.sh
  ./execute-benchmark-reachsafety-z3.sh
  ./execute-benchmark-reachsafety-mathsat.sh
  ./execute-benchmark-reachsafety-smtinterpol.sh

  cat $BENCHMARK_DIR/benchmark_memsafety_results-* > $BENCHMARK_DIR/benchmark_memsafety_results_other-solvers.txt
  cat $BENCHMARK_DIR/benchmark_reachsafety_results-* > $BENCHMARK_DIR/benchmark_reachsafety_results_other-solvers.txt
  rm -rf $BENCHMARK_DIR/benchmark_memsafety_results-*
  rm -rf $BENCHMARK_DIR/benchmark_reachsafety_results-*
fi

if [ "$num_of_cores_allowed" == "3" ]; then
  ./axdinterpolator-execute-benchmark-memsafety-z3.sh &
  pid_memsafety_z3=$!
  ./axdinterpolator-execute-benchmark-memsafety-mathsat.sh &
  pid_memsafety_mathsat=$!
  ./axdinterpolator-execute-benchmark-memsafety-smtinterpol.sh &
  pid_memsafety_smtinterpol=$!

  wait $pid_memsafety_z3
  wait $pid_memsafety_mathsat
  wait $pid_memsafety_smtinterpol

  ./axdinterpolator-execute-benchmark-reachsafety-z3.sh &
  pid_reachsafety_z3=$!
  ./axdinterpolator-execute-benchmark-reachsafety-mathsat.sh &
  pid_reachsafety_mathsat=$!
  ./axdinterpolator-execute-benchmark-reachsafety-smtinterpol.sh &
  pid_reachsafety_smtinterpol=$!

  wait $pid_reachsafety_z3
  wait $pid_reachsafety_mathsat
  wait $pid_reachsafety_smtinterpol

  cat $BENCHMARK_DIR/benchmark_memsafety_results-* > $BENCHMARK_DIR/benchmark_memsafety_results_axdinterpolator.txt
  cat $BENCHMARK_DIR/benchmark_reachsafety_results-* > $BENCHMARK_DIR/benchmark_reachsafety_results_axdinterpolator.txt
  rm -rf $BENCHMARK_DIR/benchmark_memsafety_results-*
  rm -rf $BENCHMARK_DIR/benchmark_reachsafety_results-*

  ./execute-benchmark-memsafety-z3.sh &
  pid_memsafety_z3=$!
  ./execute-benchmark-memsafety-mathsat.sh &
  pid_memsafety_mathsat=$!
  ./execute-benchmark-memsafety-smtinterpol.sh &
  pid_memsafety_smtinterpol=$!

  wait $pid_memsafety_z3
  wait $pid_memsafety_mathsat
  wait $pid_memsafety_smtinterpol

  ./execute-benchmark-reachsafety-z3.sh &
  pid_reachsafety_z3=$!
  ./execute-benchmark-reachsafety-mathsat.sh &
  pid_reachsafety_mathsat=$!
  ./execute-benchmark-reachsafety-smtinterpol.sh &
  pid_reachsafety_smtinterpol=$!

  wait $pid_reachsafety_z3
  wait $pid_reachsafety_mathsat
  wait $pid_reachsafety_smtinterpol

  cat $BENCHMARK_DIR/benchmark_memsafety_results-* > $BENCHMARK_DIR/benchmark_memsafety_results_other-solvers.txt
  cat $BENCHMARK_DIR/benchmark_reachsafety_results-* > $BENCHMARK_DIR/benchmark_reachsafety_results_other-solvers.txt
  rm -rf $BENCHMARK_DIR/benchmark_memsafety_results-*
  rm -rf $BENCHMARK_DIR/benchmark_reachsafety_results-*
fi

if [ "$num_of_cores_allowed" == "6" ]; then
  ./axdinterpolator-execute-benchmark-memsafety-z3.sh &
  pid_memsafety_z3=$!
  ./axdinterpolator-execute-benchmark-memsafety-mathsat.sh &
  pid_memsafety_mathsat=$!
  ./axdinterpolator-execute-benchmark-memsafety-smtinterpol.sh &
  pid_memsafety_smtinterpol=$!
  ./axdinterpolator-execute-benchmark-reachsafety-z3.sh &
  pid_reachsafety_z3=$!
  ./axdinterpolator-execute-benchmark-reachsafety-mathsat.sh &
  pid_reachsafety_mathsat=$!
  ./axdinterpolator-execute-benchmark-reachsafety-smtinterpol.sh &
  pid_reachsafety_smtinterpol=$!

  wait $pid_memsafety_z3
  wait $pid_memsafety_mathsat
  wait $pid_memsafety_smtinterpol
  wait $pid_reachsafety_z3
  wait $pid_reachsafety_mathsat
  wait $pid_reachsafety_smtinterpol

  cat $BENCHMARK_DIR/benchmark_memsafety_results-* > $BENCHMARK_DIR/benchmark_memsafety_results_axdinterpolator.txt
  cat $BENCHMARK_DIR/benchmark_reachsafety_results-* > $BENCHMARK_DIR/benchmark_reachsafety_results_axdinterpolator.txt
  rm -rf $BENCHMARK_DIR/benchmark_memsafety_results-*
  rm -rf $BENCHMARK_DIR/benchmark_reachsafety_results-*

  ./execute-benchmark-memsafety-z3.sh &
  pid_memsafety_z3=$!
  ./execute-benchmark-memsafety-mathsat.sh &
  pid_memsafety_mathsat=$!
  ./execute-benchmark-memsafety-smtinterpol.sh &
  pid_memsafety_smtinterpol=$!
  ./execute-benchmark-reachsafety-z3.sh &
  pid_reachsafety_z3=$!
  ./execute-benchmark-reachsafety-mathsat.sh &
  pid_reachsafety_mathsat=$!
  ./execute-benchmark-reachsafety-smtinterpol.sh &
  pid_reachsafety_smtinterpol=$!

  wait $pid_memsafety_z3
  wait $pid_memsafety_mathsat
  wait $pid_memsafety_smtinterpol
  wait $pid_reachsafety_z3
  wait $pid_reachsafety_mathsat
  wait $pid_reachsafety_smtinterpol

  cat $BENCHMARK_DIR/benchmark_memsafety_results-* > $BENCHMARK_DIR/benchmark_memsafety_results_other-solvers.txt
  cat $BENCHMARK_DIR/benchmark_reachsafety_results-* > $BENCHMARK_DIR/benchmark_reachsafety_results_other-solvers.txt
  rm -rf $BENCHMARK_DIR/benchmark_memsafety_results-*
  rm -rf $BENCHMARK_DIR/benchmark_reachsafety_results-*
fi

cat $BENCHMARK_DIR/benchmark_memsafety_results_* > $BENCHMARK_DIR/benchmark_memsafety_results.txt
cat $BENCHMARK_DIR/benchmark_reachsafety_results_* > $BENCHMARK_DIR/benchmark_reachsafety_results.txt

rm $BENCHMARK_DIR/benchmark_memsafety_results_*
rm $BENCHMARK_DIR/benchmark_reachsafety_results_*

./../results/scripts/SMT-2021-submission-results.py $TESTS_DIR/verification-files/files $BENCHMARK_DIR

rm $BENCHMARK_DIR/benchmark_memsafety_results.txt
rm $BENCHMARK_DIR/benchmark_reachsafety_results.txt

make -C ../ clean
ulimit -St unlimited -Sv unlimited
