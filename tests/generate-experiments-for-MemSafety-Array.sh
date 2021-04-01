#!/bin/bash

#c/MemSafety-Arrays

#Contains tasks for checking memory safety of array-based programs.

#The verification tasks consist of the programs that match

#array-memsafety/*.yml
#array-examples/*.yml
#array-memsafety-realloc/*.yml
#termination-crafted/Arrays*.yml
#termination-crafted/LexIndexValue*.yml
#termination-crafted/NonTermination3-1.yml
#verifythis/duplets.yml
#verifythis/elimination_max.yml
#verifythis/lcp.yml
#verifythis/prefixsum_iter.yml
#verifythis/elimination_max_rec.yml
#verifythis/elimination_max_rec_onepoint.yml

#with the specification:

#CHECK( init(main()), LTL(G valid-free) )
#CHECK( init(main()), LTL(G valid-deref) )
#CHECK( init(main()), LTL(G valid-memtrack) )

TRACK="MemSafety-Arrays"
# TODO: Fix SUBTRACKS
SUBTRACKS="array-memsafety\
  array-examples \
  array-memsafety-realloc"
subtracks=($SUBTRACKS)
PROPERTY="valid-memsafety.prp"

for SUBTRACK in $SUBTRACKS; do
  cp ./experiment-template.sh ./$TRACK-temp-$SUBTRACK.sh
  sed -i "s/_track/$TRACK/g" ./$TRACK-temp-$SUBTRACK.sh
  sed -i "s/_sub/$SUBTRACK/g" ./$TRACK-temp-$SUBTRACK.sh
  sed -i "s/_prop/$PROPERTY/g" ./$TRACK-temp-$SUBTRACK.sh
done

for i in `seq 0 2`; do
  echo "./$TRACK-temp-${subtracks[i]}.sh &"
done
wait

for SUBTRACK in $SUBTRACKS; do
  rm ./$TRACK-temp-$SUBTRACK.sh
done
