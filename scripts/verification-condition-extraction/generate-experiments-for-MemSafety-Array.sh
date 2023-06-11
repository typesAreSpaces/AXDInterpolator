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
# termination-dietlibc/*.yml
# termination-memory-alloca/*.yml

# termination-crafted/Arrays*.yml
# termination-crafted/LexIndexValue*.yml
# termination-crafted/NonTermination*.yml

# termination-crafted-lit/HeizmannHoenickeLeikePodelski-ATVA2013-Fig7.yml
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

get_num_procs_by_name(){
    local num_running=$(ps aux | grep $1 | grep -v grep | wc -l)
    echo $num_running
}

. common.sh
TRACK="MemSafety-Arrays"
PROPERTY="valid-memsafety.prp"

# ----------------------------------------
# Process files of the following subtracks
SUBTRACKS="array-fpi
  array-memsafety \
  array-examples \
  array-memsafety-realloc \
  termination-15 \
  termination-dietlibc \
  termination-memory-alloca"
subtracks=($SUBTRACKS)

# Make instantiations of the script files
# for each subtrack
for SUBTRACK in $SUBTRACKS; do
    cp ./experiment-template-subtrack.sh ./$TRACK-temp-$SUBTRACK.sh
    sed -i "s/_track_/$TRACK/g" ./$TRACK-temp-$SUBTRACK.sh
    sed -i "s/_sub_/$SUBTRACK/g" ./$TRACK-temp-$SUBTRACK.sh
    sed -i "s/_prop_/$PROPERTY/g" ./$TRACK-temp-$SUBTRACK.sh
done

for i in `seq 0 2`; do
    # Execute the scripts
    SV_COMP_DIR="$SV_COMP_DIR" \
	       ULTIMATE_DIR="$ULTIMATE_DIR" \
	       TIMEOUT_CMD="$TIMEOUT_CMD" \
	       ./$TRACK-temp-${subtracks[i]}.sh &
done
wait

# Remove the scripts
for SUBTRACK in $SUBTRACKS; do
    rm ./$TRACK-temp-$SUBTRACK.sh
done
# ----------------------------------------

# ----------------------------------------
# Process files of the following subtracks
# termination-crafted/Arrays*.yml
for FILE in $SV_COMP_DIR/termination-crafted/Arrays*.c; do 
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
    # Execute the script
    SV_COMP_DIR="$SV_COMP_DIR" \
	       ULTIMATE_DIR="$ULTIMATE_DIR" \
	       TIMEOUT_CMD="$TIMEOUT_CMD" \
	       ./$TRACK-temp-$SUBTRACK-$file_no_path.sh &
done

# Remove the script
for FILE in $SV_COMP_DIR/termination-crafted/Arrays*.c; do 
    SUBTRACK=$(basename $(dirname $FILE))
    file_no_path=$(basename $FILE)
    rm -rf ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
done
# ----------------------------------------

# ----------------------------------------
# Process files of the following subtracks
# termination-crafted/LexIndexValue*.yml
for FILE in $SV_COMP_DIR/termination-crafted/LexIndexValue*.c; do 
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
    # Execute the script
    SV_COMP_DIR="$SV_COMP_DIR" \
	       ULTIMATE_DIR="$ULTIMATE_DIR" \
	       TIMEOUT_CMD="$TIMEOUT_CMD" \
	       ./$TRACK-temp-$SUBTRACK-$file_no_path.sh &
done

# Remove the script
for FILE in $SV_COMP_DIR/termination-crafted/LexIndexValue*.c; do 
    SUBTRACK=$(basename $(dirname $FILE))
    file_no_path=$(basename $FILE)
    rm -rf ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
done
# ----------------------------------------

# ----------------------------------------
# Process files of the following subtracks
# termination-crafted/NonTermination*.yml
for FILE in $SV_COMP_DIR/termination-crafted/NonTermination*.c; do 
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
    # Execute the script
    SV_COMP_DIR="$SV_COMP_DIR" \
	       ULTIMATE_DIR="$ULTIMATE_DIR" \
	       TIMEOUT_CMD="$TIMEOUT_CMD" \
	       ./$TRACK-temp-$SUBTRACK-$file_no_path.sh &
done

# Remove the script
for FILE in $SV_COMP_DIR/termination-crafted/NonTermination*.c; do 
    SUBTRACK=$(basename $(dirname $FILE))
    file_no_path=$(basename $FILE)
    rm -rf ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
done
# ----------------------------------------

# ----------------------------------------
# Process files of the following subtracks
REMAINING_FILES="termination-crafted-lit/HeizmannHoenickeLeikePodelski-ATVA2013-Fig7.c \
  verifythis/duplets.c \
  verifythis/elimination_max.c \
  verifythis/lcp.c \
  verifythis/prefixsum_iter.c \ 
  verifythis/elimination_max_rec.c \ 
  verifythis/elimination_max_rec_onepoint.c"
for FILE in $SV_COMP_DIR/$REMAINING_FILES; do 
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
    # Execute the script
    SV_COMP_DIR="$SV_COMP_DIR" \
	       ULTIMATE_DIR="$ULTIMATE_DIR" \
	       TIMEOUT_CMD="$TIMEOUT_CMD" \
	       ./$TRACK-temp-$SUBTRACK-$file_no_path.sh &
done

# Remove the script
for FILE in $SV_COMP_DIR/$REMAINING_FILES; do 
    SUBTRACK=$(basename $(dirname $FILE))
    file_no_path=$(basename $FILE)
    rm -rf ./$TRACK-temp-$SUBTRACK-$file_no_path.sh
done
# ----------------------------------------
