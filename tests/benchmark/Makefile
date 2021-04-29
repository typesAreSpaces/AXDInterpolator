CURRENT_DIR=$(shell pwd)
IDIR=$(CURRENT_DIR)/include
ODIR=$(CURRENT_DIR)/obj
SDIR=$(CURRENT_DIR)/src
LDIR=$(CURRENT_DIR)/../../lib
CC=g++
OS=$(shell uname)
ifeq ($(OS), Darwin)
	SO_EXT=dylib
	DYLD_LIBRARY_PATH=$(LDIR)
	export DYLD_LIBRARY_PATH
	NUM_PROCS=$(shell sysctl -n hw.logicalcpu)
else
	SO_EXT=so
	NUM_PROCS=$(shell nproc)
endif
NUM_PROCS_H=$$(($(NUM_PROCS)/2))

SRC=$(wildcard $(SDIR)/*.cpp)
DEPS=$(wildcard $(IDIR)/*.h)
OBJS=$(SRC:$(SDIR)/%.cpp=$(ODIR)/%.o) $(LDIR)/libz3.$(SO_EXT)
FLAGS=-I$(SDIR) -I$(IDIR) -std=c++11 -Wall

FILE_TEST=/media/Documents/MaxDiff-Experiments/verification-files/MemSafety-Arrays/array-examples/relax-1.c_valid-memsafety.prp.smt2
#FILE_TEST=/media/Documents/MaxDiff-Experiments/verification-files/MemSafety-Arrays/array-examples/data_structures_set_multi_proc_trivial_ground.c_valid-memsafety.prp.smt2
#FILE_TEST=/media/Documents/MaxDiff-Experiments/verification-files/ReachSafety-Arrays/array-cav19/array_tiling_poly6.c_unreach-call.prp.smt2

all: test

# --------------------------------------------------------
#  Rules to build the project
$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) $(LDIR)/libz3.$(SO_EXT)
	@mkdir -p $(CURRENT_DIR)/obj
	$(CC) -g -c -o $@ $(FLAGS) $<

../../bin/axd_interpolator:
	make -j8 -C ../../ bin/axd_interpolator

bin/benchmark: $(OBJS) $(LDIR)/libz3.$(SO_EXT) ../../bin/axd_interpolator
	@mkdir -p $(CURRENT_DIR)/bin
	$(CC) -g -o $@ $(OBJS) $(FLAGS) -lpthread
# --------------------------------------------------------

# ------------------------------------------
#  Rules to test a single or many smt2 files
test: bin/benchmark ../../bin/axd_interpolator
	$(CURRENT_DIR)/bin/benchmark $(FILE_TEST) 1 $(CURRENT_DIR)/output.txt
# ------------------------------------------

# ----------------------
#  Cleaning
.PHONY: clean
clean:
	make -C ../../ clean
	rm -rf *.smt2
	rm -rf $(ODIR)/*
	rm -rf $(CURRENT_DIR)/bin/benchmark
# ----------------------
