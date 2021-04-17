IDIR=./include
ODIR=./obj
SDIR=./src
LDIR=./../../lib
CURRENT_DIR=$(shell pwd)
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

all: main

# --------------------------------------------------------
#  Rules to build the project
$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) $(LDIR)/libz3.$(SO_EXT)
	@mkdir -p ./obj
	$(CC) -g -c -o $@ $(FLAGS) $<

./bin/benchmark: $(OBJS) $(LDIR)/libz3.$(SO_EXT)
	@mkdir -p ./bin
	$(CC) -g -o $@ $(OBJS) $(FLAGS) -lpthread
# --------------------------------------------------------

# ------------------------------------------
#  Rules to test a single or many smt2 files
main: ./bin/benchmark
	echo "haha"
	#./bin/benchmark
# ------------------------------------------

# ----------------------
#  Cleaning
.PHONY: clean
clean:
	rm -rf $(ODIR)/*
	rm -rf ./bin/benchmark
# ----------------------
