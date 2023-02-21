Z3_IDIR=$(CURRENT_DIR)/include
ODIR=$(CURRENT_DIR)/obj
SDIR=$(CURRENT_DIR)/src
LDIR=$(CURRENT_DIR)/lib
BDIR=$(CURRENT_DIR)/bin

TEST_DIR=$(CURRENT_DIR)/tests/smt2-files
Z3_DIR=$(CURRENT_DIR)/dependencies/z3-interp-plus

AXD_INTERPOLATOR=$(BDIR)/axd_interpolator
TAGS=$(CURRENT_DIR)/compile_commands.json

CXX=g++
CXXFLAGS= 
CCFLAGS=
PYTHON_CMD=python3

OS=$(shell uname)
ifeq ($(OS), Darwin)
	DYLD_LIBRARY_PATH=$(LDIR)
	export DYLD_LIBRARY_PATH
	SO_EXT=dylib
	_NUM_PROCS=$(shell sysctl -n hw.logicalcpu)
endif
ifeq ($(OS), Linux)
	SO_EXT=so
	_NUM_PROCS=$(shell nproc)
endif
_NUM_PROCS_H=$$(($(NUM_PROCS)/2))
NUM_PROCS=$(_NUM_PROCS)

Z3_HEADERS=$(wildcard $(Z3_IDIR)/*.h)

FLAGS=-std=c++11 -Wall

