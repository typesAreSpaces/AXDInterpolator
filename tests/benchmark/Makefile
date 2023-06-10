CURRENT_DIR=$(shell pwd)
IDIR=$(CURRENT_DIR)/include
ODIR=$(CURRENT_DIR)/obj
SDIR=$(CURRENT_DIR)/src
BDIR=$(CURRENT_DIR)/bin
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

Z3_SOLVER_CODE=0
MATHSAT_SOLVER_CODE=1
SMTINTERPOL_SOLVER_CODE=2

TEST_OUR_IMP=0
TEST_OTHER_IMP=1

TAGS=compile_commands.json

AXD_INTERPOLATOR_PREFIX_PATH=../..
AXD_INTERPOLATOR=$(AXD_INTERPOLATOR_PREFIX_PATH)/bin/axd_interpolator
BIN_BENCHMARK=$(BDIR)/benchmark
OUTPUT_FILE=$(CURRENT_DIR)/output.txt 

FILE_TEST_PREFIX_PATH=./files
FILE_TEST=$(FILE_TEST_PREFIX_PATH)/MemSafety-Arrays/array-examples/relax-2.c_valid-memsafety.prp.smt2
#FILE_TEST=$(FILE_TEST_PREFIX_PATH)/MemSafety-Arrays/array-examples/data_structures_set_multi_proc_trivial_ground.c_valid-memsafety.prp.smt2
#FILE_TEST=$(FILE_TEST_PREFIX_PATH)/ReachSafety-Arrays/array-cav19/array_tiling_poly6.c_unreach-call.prp.smt2

all: test
	echo $(BIN_BENCHMARK)

# --------------------------------------------------------
#  Rules to build the project
$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) $(LDIR)/libz3.$(SO_EXT)
	@mkdir -p $(CURRENT_DIR)/obj
	$(CC) -g -c -o $@ $(FLAGS) $<

$(AXD_INTERPOLATOR):
	make -j8 --directory=$(AXD_INTERPOLATOR_PREFIX_PATH)/

$(LDIR)/libz3.$(SO_EXT): $(AXD_INTERPOLATOR)
# --------------------------------------------------------

# -------------------------------------------------------------------
$(BIN_BENCHMARK): $(OBJS) $(LDIR)/libz3.$(SO_EXT) $(AXD_INTERPOLATOR)
	@mkdir -p $(BDIR)
	$(CC) -g -o $@ $(OBJS) $(FLAGS) -lpthread
# -------------------------------------------------------------------

# ----------------------------------------------
#  Rules to test a single or many smt2 files
test: $(BIN_BENCHMARK) $(AXD_INTERPOLATOR)
	$(BIN_BENCHMARK) $(FILE_TEST) \
		$(Z3_SOLVER_CODE) $(OUTPUT_FILE)\
		$(TEST_OUR_IMP)
# ----------------------------------------------

# -----------------
# Generate TAGS
$(TAGS):
	compiledb -n make
# -----------------

# --------------------------------------
#  Cleaning
.PHONY: clean

clean:
	make -C $(AXD_INTERPOLATOR_PREFIX_PATH)/ clean
	rm -rf *.smt2
	rm -rf $(ODIR) $(BDIR)
	rm -rf $(BIN_BENCHMARK)
	rm -rf $(TAGS) $(OUTPUT_FILE)
# --------------------------------------
