#!/bin/bash

. common.sh

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

FILE_OUT=reachsafety-test-smtinterpol.txt
rm -rf $FILE_OUT
time ./caxdinterpolator-execute-benchmark-reachsafety-smtinterpol.sh > $FILE_OUT

# Rename stuff (?)
cat $BENCHMARK_DIR/benchmark_memsafety_results-* > $BENCHMARK_DIR/benchmark_memsafety_results_caxdinterpolator.txt
cat $BENCHMARK_DIR/benchmark_reachsafety_results-* > $BENCHMARK_DIR/benchmark_reachsafety_results_caxdinterpolator.txt
rm -rf $BENCHMARK_DIR/benchmark_memsafety_results-*
rm -rf $BENCHMARK_DIR/benchmark_reachsafety_results-*

# Rename stuff (?)
cat $BENCHMARK_DIR/benchmark_memsafety_results_* > $BENCHMARK_DIR/benchmark_memsafety_results.txt
cat $BENCHMARK_DIR/benchmark_reachsafety_results_* > $BENCHMARK_DIR/benchmark_reachsafety_results.txt

rm $BENCHMARK_DIR/benchmark_memsafety_results_*
rm $BENCHMARK_DIR/benchmark_reachsafety_results_*

# Post-processing script
#. get_table.sh

make -C $BENCHMARK_DIR clean

ulimit -St unlimited -Sv unlimited
