#!/bin/bash

CURRENT_DIR=$(pwd)
BENCHMARK_DIR=$(dirname $CURRENT_DIR)
TESTS_DIR=$(dirname $BENCHMARK_DIR)
ROOT_DIR=$(dirname $TESTS_DIR)
# VERIFICATION_FILES_DIR=$(dirname $BENCHMARK_DIR)/verification-files/files
BENCHMARK_FILES_DIR=$ROOT_DIR/tests/smt2-files/benchmark
VERIFICATION_FILES_DIR=$BENCHMARK_FILES_DIR/files

# Unzip 'raw' verification files
# in $BENCHMARK_FILES_DIR
# if such directory is not present
[ ! -d $VERIFICATION_FILES_DIR ] \
  && unzip $VERIFICATION_FILES_DIR.zip

# ---------------------------
# Move to the root of project
cd $ROOT_DIR

# Abort if mathsat bin is not present
[ ! -f ./bin/mathsat ] \
    && echo "Mathsat not in local bin folder" \
    && cd $CURRENT_DIR \
    && exit

echo "How many cores can be used? 1, 3, or 6"
read num_of_cores_allowed

# Download z3-interp-plus if not present
[ -z "$(ls -A ./dependencies/z3-interp-plus)" ] && git submodule update --init --remote dependencies/z3-interp-plus
# ---------------------------

# Move to directory containing files.zip
# cd ./tests/verification-files
cd $BENCHMARK_DIR

# Execute main benchmark script
make -j8 bin/benchmark

# Move to 'scripts' directory to post-process the results
cd $CURRENT_DIR

ulimit -St 360 -Sv 4500000

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
  # Execute scripts to test axdinterpolant using z3
  # for the memsafety track
  ./axdinterpolator-execute-benchmark-memsafety-z3.sh &
  pid_memsafety_z3=$!
  # Execute scripts to test axdinterpolant using mathsat
  # for the memsafety track
  ./axdinterpolator-execute-benchmark-memsafety-mathsat.sh &
  pid_memsafety_mathsat=$!
  # Execute scripts to test axdinterpolant using smtinterpol
  # for the memsafety track
  ./axdinterpolator-execute-benchmark-memsafety-smtinterpol.sh &
  pid_memsafety_smtinterpol=$!
  # Execute scripts to test axdinterpolant using z3
  # for the reachsafety track
  ./axdinterpolator-execute-benchmark-reachsafety-z3.sh &
  pid_reachsafety_z3=$!
  # Execute scripts to test axdinterpolant using mathsat
  # for the reachsafety track
  ./axdinterpolator-execute-benchmark-reachsafety-mathsat.sh &
  pid_reachsafety_mathsat=$!
  # Execute scripts to test axdinterpolant using smtinterpol
  # for the reachsafety track
  ./axdinterpolator-execute-benchmark-reachsafety-smtinterpol.sh &
  pid_reachsafety_smtinterpol=$!

  # Join for the async executions to finish
  wait $pid_memsafety_z3
  wait $pid_memsafety_mathsat
  wait $pid_memsafety_smtinterpol
  wait $pid_reachsafety_z3
  wait $pid_reachsafety_mathsat
  wait $pid_reachsafety_smtinterpol

  # Rename stuff (?)
  cat $BENCHMARK_DIR/benchmark_memsafety_results-* > $BENCHMARK_DIR/benchmark_memsafety_results_axdinterpolator.txt
  cat $BENCHMARK_DIR/benchmark_reachsafety_results-* > $BENCHMARK_DIR/benchmark_reachsafety_results_axdinterpolator.txt
  rm -rf $BENCHMARK_DIR/benchmark_memsafety_results-*
  rm -rf $BENCHMARK_DIR/benchmark_reachsafety_results-*

  # Execute scripts to test z3
  # for the memsafety track
  ./execute-benchmark-memsafety-z3.sh &
  pid_memsafety_z3=$!
  # Execute scripts to test mathsat
  # for the memsafety track
  ./execute-benchmark-memsafety-mathsat.sh &
  pid_memsafety_mathsat=$!
  # Execute scripts to test smtinterpol
  # for the memsafety track
  ./execute-benchmark-memsafety-smtinterpol.sh &
  pid_memsafety_smtinterpol=$!
  # Execute scripts to test z3
  # for the reachsafety track
  ./execute-benchmark-reachsafety-z3.sh &
  pid_reachsafety_z3=$!
  # Execute scripts to test mathsat
  # for the reachsafety track
  ./execute-benchmark-reachsafety-mathsat.sh &
  pid_reachsafety_mathsat=$!
  # Execute scripts to test smtinterpol
  # for the reachsafety track
  ./execute-benchmark-reachsafety-smtinterpol.sh &
  pid_reachsafety_smtinterpol=$!

  # Join for the async executions to finish
  wait $pid_memsafety_z3
  wait $pid_memsafety_mathsat
  wait $pid_memsafety_smtinterpol
  wait $pid_reachsafety_z3
  wait $pid_reachsafety_mathsat
  wait $pid_reachsafety_smtinterpol

  # Rename stuff (?)
  cat $BENCHMARK_DIR/benchmark_memsafety_results-* > $BENCHMARK_DIR/benchmark_memsafety_results_other-solvers.txt
  cat $BENCHMARK_DIR/benchmark_reachsafety_results-* > $BENCHMARK_DIR/benchmark_reachsafety_results_other-solvers.txt
  rm -rf $BENCHMARK_DIR/benchmark_memsafety_results-*
  rm -rf $BENCHMARK_DIR/benchmark_reachsafety_results-*
fi

# Rename stuff (?)
cat $BENCHMARK_DIR/benchmark_memsafety_results_* > $BENCHMARK_DIR/benchmark_memsafety_results.txt
cat $BENCHMARK_DIR/benchmark_reachsafety_results_* > $BENCHMARK_DIR/benchmark_reachsafety_results.txt

rm $BENCHMARK_DIR/benchmark_memsafety_results_*
rm $BENCHMARK_DIR/benchmark_reachsafety_results_*

# Post-processing script
$CURRENT_DIR/process-results/SMT-2021-submission-results.py $VERIFICATION_FILES_DIR $BENCHMARK_DIR

#rm $BENCHMARK_DIR/benchmark_memsafety_results.txt
#rm $BENCHMARK_DIR/benchmark_reachsafety_results.txt

make -C $BENCHMARK_DIR clean

ulimit -St unlimited -Sv unlimited
