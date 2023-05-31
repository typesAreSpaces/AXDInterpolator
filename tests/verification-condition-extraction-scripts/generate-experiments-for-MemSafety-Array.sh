#!/bin/bash

# * https://sv-comp.sosy-lab.org/2023/benchmarks.php

# c/MemSafety-Arrays

# Contains tasks for checking memory safety of array-based programs.

# The verification tasks consist of the programs that match

# array-fpi/*.yml
# array-memsafety/*.yml
# array-examples/*.yml
# array-memsafety-realloc/*.yml
# termination-15/*.yml
# termination-crafted-lit/HeizmannHoenickeLeikePodelski-ATVA2013-Fig7.yml
# termination-crafted/Arrays*.yml
# termination-crafted/LexIndexValue*.yml
# termination-crafted/NonTermination*.yml
# termination-dietlibc/*.yml
# termination-memory-alloca/*.yml
# verifythis/duplets.yml
# verifythis/elimination_max.yml
# verifythis/lcp.yml
# verifythis/prefixsum_iter.yml
# verifythis/elimination_max_rec.yml
# verifythis/elimination_max_rec_onepoint.yml
# with the specification:

# CHECK( init(main()), LTL(G valid-free) )
# CHECK( init(main()), LTL(G valid-deref) )
# CHECK( init(main()), LTL(G valid-memtrack) )

# * Old (reference)

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

get_num_procs_by_name(){
  local num_running=$(ps aux | grep $1 | grep -v grep | wc -l)
  echo $num_running
}

TRACK="MemSafety-Arrays"
SUBTRACKS="array-memsafety\
  array-examples \
  array-memsafety-realloc"
subtracks=($SUBTRACKS)
PROPERTY="valid-memsafety.prp"

for SUBTRACK in $SUBTRACKS; do
  cp ./experiment-template-subtrack.sh ./$TRACK-temp-$SUBTRACK.sh
  sed -i "s/_track_/$TRACK/g" ./$TRACK-temp-$SUBTRACK.sh
  sed -i "s/_sub_/$SUBTRACK/g" ./$TRACK-temp-$SUBTRACK.sh
  sed -i "s/_prop_/$PROPERTY/g" ./$TRACK-temp-$SUBTRACK.sh
done

for i in `seq 0 2`; do
  ./$TRACK-temp-${subtracks[i]}.sh &
done
wait

for SUBTRACK in $SUBTRACKS; do
  rm ./$TRACK-temp-$SUBTRACK.sh
done

# termination-crafted/Arrays*.yml
for FILE in ./sv-benchmarks/c/termination-crafted/Arrays*.c; do 
  SUBTRACK=$(basename $(dirname $FILE))
  file_no_path=$(basename $FILE)
  cp ./experiment-template-file.sh ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_track_/$TRACK/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_sub_/$SUBTRACK/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_prop_/$PROPERTY/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_file_/${FILE//\//\\/}/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  while [ "$(get_num_procs_by_name temp)" -gt 7 ]; do
    sleep 1
  done
  ./$TRACK-temp-$SUBTRACK-$file_no_path.sh &
done

for FILE in ./sv-benchmarks/c/termination-crafted/Arrays*.c; do 
  SUBTRACK=$(basename $(dirname $FILE))
  file_no_path=$(basename $FILE)
  rm -rf ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
done

# termination-crafted/LexIndexValue*.yml
for FILE in ./sv-benchmarks/c/termination-crafted/LexIndexValue*.c; do 
  SUBTRACK=$(basename $(dirname $FILE))
  file_no_path=$(basename $FILE)
  cp ./experiment-template-file.sh ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_track_/$TRACK/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_sub_/$SUBTRACK/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_prop_/$PROPERTY/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_file_/${FILE//\//\\/}/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  while [ "$(get_num_procs_by_name temp)" -gt 7 ]; do
    sleep 1
  done
  ./$TRACK-temp-$SUBTRACK-$file_no_path.sh &
done

for FILE in ./sv-benchmarks/c/termination-crafted/LexIndexValue*.c; do 
  SUBTRACK=$(basename $(dirname $FILE))
  file_no_path=$(basename $FILE)
  rm -rf ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
done

REMAINING_FILES="./termination-crafted/NonTermination3-1.c \
  ./verifythis/duplets.c \
  ./verifythis/elimination_max.c \
  ./verifythis/lcp.c \
  ./verifythis/prefixsum_iter.c \ 
  ./verifythis/elimination_max_rec.c \ 
  ./verifythis/elimination_max_rec_onepoint.c"
for FILE in $REMAINING_FILES; do 
  SUBTRACK=$(basename $(dirname $FILE))
  file_no_path=$(basename $FILE)
  cp ./experiment-template-file.sh ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_track_/$TRACK/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_sub_/$SUBTRACK/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_prop_/$PROPERTY/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  sed -i "s/_file_/${FILE//\//\\/}/g" ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
  while [ "$(get_num_procs_by_name temp)" -gt 7 ]; do
    sleep 1
  done
  ./$TRACK-temp-$SUBTRACK-$file_no_path.sh &
done

for FILE in $REMAINING_FILES; do 
  SUBTRACK=$(basename $(dirname $FILE))
  file_no_path=$(basename $FILE)
  rm -rf ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
done
