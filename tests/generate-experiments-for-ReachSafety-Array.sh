#!/bin/bash

#c/ReachSafety-Arrays

#Contains tasks for which treatment of arrays is necessary in order to determine reachability.

#The verification tasks consist of the programs that match

#array-examples/*.yml
#array-industry-pattern/*.yml
#reducercommutativity/*.yml
#array-tiling/*.yml
#array-programs/*.yml
#array-crafted/*.yml
#array-multidimensional/*.yml
#array-patterns/*.yml
#array-cav19/*.yml
#array-lopstr16/*.yml
#array-fpi/*.yml

#with the specification:

#CHECK( init(main()), LTL(G ! call(reach_error())) )

TRACK="ReachSafety-Arrays"
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
PROPERTY="unreach-call.prp"

for SUBTRACK in $SUBTRACKS; do
  cp ./experiment-template.sh ./$TRACK-temp-$SUBTRACK.sh
  sed -i "s/_track/$TRACK/g" ./$TRACK-temp-$SUBTRACK.sh
  sed -i "s/_sub/$SUBTRACK/g" ./$TRACK-temp-$SUBTRACK.sh
  sed -i "s/_prop/$PROPERTY/g" ./$TRACK-temp-$SUBTRACK.sh
done

for i in `seq 0 6`; do
  ./$TRACK-temp-${subtracks[i]}.sh &
done
wait

for i in `seq 7 10`; do
  ./$TRACK-temp-${subtracks[i]}.sh &
done
wait

for SUBTRACK in $SUBTRACKS; do
  rm ./$TRACK-temp-$SUBTRACK.sh
done
