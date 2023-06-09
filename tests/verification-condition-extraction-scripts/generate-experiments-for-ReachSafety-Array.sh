#!/bin/bash

# * https://sv-comp.sosy-lab.org/2023/benchmarks.php

# c/ReachSafety-Arrays

# Contains tasks for which treatment of arrays is necessary in order to determine reachability.

# The verification tasks consist of the programs that match

# array-examples/*.yml
# array-industry-pattern/*.yml
# reducercommutativity/*.yml
# array-tiling/*.yml
# array-programs/*.yml
# array-crafted/*.yml
# array-multidimensional/*.yml
# array-patterns/*.yml
# array-cav19/*.yml
# array-lopstr16/*.yml
# array-fpi/*.yml
# with the specification:

# CHECK( init(main()), LTL(G ! call(reach_error())) )

. common.sh
TRACK="ReachSafety-Arrays"
PROPERTY="unreach-call.prp"

# ----------------------------------------
# Process files of the following subtracks
SUBTRACKS="array-examples \
  array-industry-pattern \
  reducercommutativity \
  array-tiling \
  array-programs \
  array-crafted \
  array-multidimensional \
  array-patterns \
  array-cav19 \
  array-lopstr16 \
  array-fpi"
subtracks=($SUBTRACKS)

# Make instantiations of the script files
# for each subtrack
for SUBTRACK in $SUBTRACKS; do
    cp ./experiment-template-subtrack.sh ./$TRACK-temp-$SUBTRACK.sh
    sed -i "s/_track_/$TRACK/g" ./$TRACK-temp-$SUBTRACK.sh
    sed -i "s/_sub_/$SUBTRACK/g" ./$TRACK-temp-$SUBTRACK.sh
    sed -i "s/_prop_/$PROPERTY/g" ./$TRACK-temp-$SUBTRACK.sh
done

for i in `seq 0 6`; do
    # Execute the scripts
    SV_COMP_DIR="$SV_COMP_DIR" ULTIMATE_DIR="$ULTIMATE_DIR" \
	       ./$TRACK-temp-${subtracks[i]}.sh &
done
wait

for i in `seq 7 10`; do
    # Execute the scripts
    SV_COMP_DIR="$SV_COMP_DIR" ULTIMATE_DIR="$ULTIMATE_DIR" \
	       ./$TRACK-temp-${subtracks[i]}.sh &
done
wait

# Remove the scripts
for SUBTRACK in $SUBTRACKS; do
    rm ./$TRACK-temp-$SUBTRACK.sh
done
# ----------------------------------------
