#!/usr/bin/env bash

ulimit -St 360 -Sv 4500000

for FILE in ./tests/smt2-files/problematic_files_caxdinterpolator_smtinterpol/*.smt2; do
    echo "Processing $FILE"
    time ./bin/caxd_interpolator QF_LIA $FILE 2
done

ulimit -St unlimited -Sv unlimited
