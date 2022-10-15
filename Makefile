CURRENT_DIR=$(shell pwd)
IDIR=$(CURRENT_DIR)/include
ODIR=$(CURRENT_DIR)/obj
SDIR=$(CURRENT_DIR)/src
LDIR=$(CURRENT_DIR)/lib
BDIR=$(CURRENT_DIR)/bin
TEST_DIR=$(CURRENT_DIR)/tests/smt2-files
Z3_DIR=dependencies/z3-interp-plus
AXD_INTERPOLATOR=bin/axd_interpolator

CXX=g++
CXXFLAGS= 
CCFLAGS=

OS=$(shell uname)
ifeq ($(OS), Darwin)
	PYTHON_CMD=python
	SO_EXT=dylib
	DYLD_LIBRARY_PATH=$(LDIR)
	export DYLD_LIBRARY_PATH
	_NUM_PROCS=$(shell sysctl -n hw.logicalcpu)
endif
ifeq ($(OS), Linux)
	PYTHON_CMD=python
	SO_EXT=so
	_NUM_PROCS=$(shell nproc)
endif
_NUM_PROCS_H=$$(($(NUM_PROCS)/2))
NUM_PROCS=$(_NUM_PROCS)

SRC=$(wildcard $(SDIR)/*.cpp)
_DEPS=$(wildcard $(IDIR)/*.h)
DEPS:=$(filter-out $(IDIR)/AXDInterpolant.h,$(_DEPS)) \
			$(IDIR)/_AXDInterpolant.h
OBJS=$(SRC:$(SDIR)/%.cpp=$(ODIR)/%.o) $(LDIR)/libz3.$(SO_EXT)
FLAGS=-I$(SDIR) -I$(IDIR) -std=c++11 -Wall

METHOD=0# Z3
#METHOD=1# MATHSAT
#METHOD=2# SMTINTERPOL

ALLOWED_ATTEMPS=1000000

#-- Supported Theories
#THEORY=QF_TO
#THEORY=QF_IDL
#THEORY=QF_UTVPI
THEORY=QF_LIA

#-- Sample files
#FILE_TEST=$(TEST_DIR)/relax-1.c_valid-memsafety.prp.smt2
#FILE_TEST=$(TEST_DIR)/array_tiling_poly6.c_unreach-call.prp.smt2
#FILE_TEST=$(TEST_DIR)/simple.smt2
#FILE_TEST=$(TEST_DIR)/simple2.smt2
#FILE_TEST=$(TEST_DIR)/simple3.smt2
#FILE_TEST=$(TEST_DIR)/simple4.smt2
#FILE_TEST=$(TEST_DIR)/ijcar_2018_paper_example4_n_4.smt2
#FILE_TEST=$(TEST_DIR)/length_example.smt2
#FILE_TEST=$(TEST_DIR)/maxdiff_paper_example_compact.smt2
#FILE_TEST=$(TEST_DIR)/maxdiff_paper_example_another_another.smt2
#FILE_TEST=$(TEST_DIR)/maxdiff_paper_example.smt2
#FILE_TEST=$(TEST_DIR)/jhala.smt2
FILE_TEST=$(TEST_DIR)/strcpy_example_variant_1.smt2
#FILE_TEST=$(TEST_DIR)/strcpy_example_variant_2.smt2
#FILE_TEST=$(TEST_DIR)/strcpy_example_variant_3.smt2

all: tests/one
#all: tests/all
#all: tests/print_all

# ---------------------------------------------------------
#  Rules to build the project

$(CURRENT_DIR)/$(Z3_DIR)/README.md:
	git submodule update --init --remote $(Z3_DIR) 
	cd $(Z3_DIR); git checkout master

$(LDIR)/libz3.$(SO_EXT): $(CURRENT_DIR)/$(Z3_DIR)/README.md
	mkdir -p $(LDIR)
	cd $(Z3_DIR);\
		$(PYTHON_CMD) scripts/mk_make.py --prefix=$(CURRENT_DIR);\
		cd build; make install -j$(NUM_PROCS)

$(IDIR)/_AXDInterpolant.h: $(IDIR)/AXDInterpolant.h
	perl -pe "s|replace_once|$(CURRENT_DIR)|g" $< > $@

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) $(LDIR)/libz3.$(SO_EXT)
	mkdir -p $(ODIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) $<

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: $(AXD_INTERPOLATOR)

$(AXD_INTERPOLATOR): $(OBJS) $(LDIR)/libz3.$(SO_EXT)
	mkdir -p $(BDIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(FLAGS) -lpthread
# ---------------------------------------------------------

# ---------------------------------------------------------
# Generate TAGS
compile_commands.json:
	compiledb -n make
# ---------------------------------------------------------

# ---------------------------------------------------------
#  Rules to test a single or many smt2 files
tests/one: $(AXD_INTERPOLATOR)
	./$(AXD_INTERPOLATOR) \
		$(THEORY) $(FILE_TEST) $(METHOD) $(ALLOWED_ATTEMPS)
	rm -rf tests/*.o $@

tests/all: $(AXD_INTERPOLATOR)
	for smt_file in $(TEST_DIR)/*.smt2; do \
		./$(AXD_INTERPOLATOR) \
		$(THEORY) $${smt_file} $(METHOD) $(ALLOWED_ATTEMPS) ; \
		done
	rm -rf tests/*.o $@

tests/print_all: $(AXD_INTERPOLATOR)
	for smt_file in $(TEST_DIR)/*.smt2; do \
		if [ "${METHOD}" = "0" ]; \
		then METHOD_NAME="Z3"; \
		else \
		if [ "${METHOD}" = "1" ]; \
		then METHOD_NAME="MATHSAT"; \
		else METHOD_NAME="SMTINTERPOL"; \
		fi \
		fi; \
		./$(AXD_INTERPOLATOR) \
		$(THEORY) $${smt_file} $(METHOD) $(ALLOWED_ATTEMPS) \
		> $${smt_file}_${THEORY}_$${METHOD_NAME}_output.txt ; \
		done
	rm -rf tests/*.o $@
# ---------------------------------------------------------

# ---------------------------------------------------------
#  Check output
check: 
	make -C ./output

mathsat_check: 
	SMT_SOLVER=MATHSAT make check

z3_check: 
	SMT_SOLVER=Z3 make check
# ---------------------------------------------------------

# ---------------------------------------------------------
#  Cleaning
.PHONY: clean
clean:
	rm -rf $(ODIR) output/*.smt2
	rm -rf $(TEST_DIR)/*.txt
	rm -rf $(CURRENT_DIR)/$(AXD_INTERPOLATOR)
	cd output; make clean
	rm -rf compile_commands.json

.PHONY: z3_clean
z3_clean:
	cd $(Z3_DIR)/build;\
		make uninstall
	rm -rf $(CURRENT_DIR)/$(Z3_DIR)
	rm -rf $(LDIR)

.PHONY: deep_clean
deep_clean: clean z3_clean
# ---------------------------------------------------------
