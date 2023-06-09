#!/bin/bash

TRACK="_track_"
SUBTRACK="_sub_"
PROPERTY="_prop_"
FILE="_file_"

MODE=simple
ULTIMATE_CMD=$ULTIMATE_DIR/Ultimate.py
ULTIMATE_ARGS_32="--spec ../properties/$PROPERTY --architecture 32bit $MODE"
ULTIMATE_ARGS_64="--spec ../properties/$PROPERTY --architecture 64bit $MODE"
TIMEOUT_CMD='timeout 300'
LOG_FILE=$PWD/run_experiment-$TRACK-Log-$SUBTRACK.txt

if [ ! -f $LOG_FILE ]; then 
    touch $LOG_FILE
    echo "Track $TRACK - Subtrack $SUBTRACK experiments" >> $LOG_FILE
fi

ROOT_DIR=$PWD
if [ ! -d $ROOT_DIR/$TRACK/$SUBTRACK ]; then 
    mkdir -p $ROOT_DIR/$TRACK/$SUBTRACK
fi

CURR_DIR=$(dirname $FILE)
pushd $CURR_DIR > /dev/null
file_no_path=$(basename $FILE) 
yml_file="${file_no_path/.c/.yml}"

if [ ! -f $yml_file ]; then
    echo "YML file not found for file $file_no_path ." >> $LOG_FILE
    continue
fi

arch=$(yq --raw-output "select(.options != null) | .options.data_model" $yml_file)

unset ultimate_exit_code

echo ">>> Processing file: $FILE"

if [ "$arch" = "ILP32" ]; then 
    $TIMEOUT_CMD $ULTIMATE_CMD $ULTIMATE_ARGS_32 --file $file_no_path
    ultimate_exit_code=$?
fi
if [ "$arch" = "LP64" ]; then 
    $TIMEOUT_CMD $ULTIMATE_CMD $ULTIMATE_ARGS_64 --file $file_no_path
    ultimate_exit_code=$?
fi

if [ -z ${ultimate_exit_code+x} ]; then
    echo "Architecture not supported for file $file_no_path with property $PROPERTY" >> $LOG_FILE
    continue
else
    if [ $ultimate_exit_code -eq 0 ]; then
	echo "UAutomizer finished for file $file_no_path , property $PROPERTY , arch $arch" >> $LOG_FILE
	rm -rf Ultimate.log UltimateCounterExample.errorpath
	# Ultimate UAutomizer is configured to dump the SMT2 script at $HOME
	if [ -f $HOME/$file_no_path.smt2 ]; then
	    mv $HOME/$file_no_path.smt2 $ROOT_DIR/$TRACK/$SUBTRACK/${file_no_path}_${PROPERTY}.smt2
	fi
    else
	if [ $ultimate_exit_code -eq 124 ]; then
	    echo "UAutomizer timed out for file $file_no_path , property $PROPERTY , arch $arch" >> $LOG_FILE
	    rm -rf Ultimate.log UltimateCounterExample.errorpath $HOME/$file_no_path.smt2
	else
	    echo "Unexpected exit code $ultimate_exit_code for file $file_no_path , property $PROPERTY , arch $arch" >> $LOG_FILE
	fi
    fi
fi

popd > /dev/null

