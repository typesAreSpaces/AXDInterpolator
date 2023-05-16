#!/usr/bin/env bash

COMMON_FILE=common.mk

msg(){
  echo "$1" >> $COMMON_FILE
}

rm -rf $COMMON_FILE
touch $COMMON_FILE

msg "CURRENT_DIR=$(pwd)" 
msg "Z3_IDIR=\$(CURRENT_DIR)/include" 
msg "ODIR=\$(CURRENT_DIR)/obj" 
msg "SDIR=\$(CURRENT_DIR)/src" 
msg "LDIR=\$(CURRENT_DIR)/lib" 
msg "BDIR=\$(CURRENT_DIR)/bin" 
msg ""
msg "TEST_DIR=\$(CURRENT_DIR)/tests/smt2-files" 
msg "Z3_DIR=\$(CURRENT_DIR)/dependencies/z3-interp-plus" 
msg ""
msg "CAXD_INTERPOLATOR=\$(BDIR)/caxd_interpolator" 
msg "TAGS=\$(CURRENT_DIR)/compile_commands.json" 
msg ""
msg "CXX=g++" 
msg "CXXFLAGS= " 
msg "CCFLAGS=" 
msg "PYTHON_CMD=python3" 
msg ""
msg "OS=\$(shell uname)" 
msg "ifeq (\$(OS), Darwin)" 
msg "  DYLD_LIBRARY_PATH=\$(LDIR)" 
msg "  export DYLD_LIBRARY_PATH" 
msg "  SO_EXT=dylib" 
msg "  _NUM_PROCS=\$(shell sysctl -n hw.logicalcpu)" 
msg "endif" 
msg "ifeq (\$(OS), Linux)" 
msg "  SO_EXT=so" 
msg "  _NUM_PROCS=\$(shell nproc)" 
msg "endif" 
msg "_NUM_PROCS_H=\$\$((\$(NUM_PROCS)/2))" 
msg "NUM_PROCS=\$(_NUM_PROCS)" 
msg ""
msg "Z3_HEADERS=\$(wildcard \$(Z3_IDIR)/*.h)" 
msg ""
msg "FLAGS=-std=c++11 -Wall -pedantic" 
