IDIR=./include
ODIR=./obj
SDIR=./src
LDIR=./lib
CC=g++
COMPILERVERSION=$(shell g++ -dumpversion)
LINUXOLDCOMPVERSION=8.3.0
FLAGS=-I$(SDIR) -I$(IDIR) -std=c++11 -Wall

SRC=$(wildcard $(SDIR)/*.cpp)
OBJS=$(SRC:$(SDIR)/%.cpp=$(ODIR)/%.o) $(LDIR)/libz3.so
DEPS=$(wildcard $(IDIR)/*.h)
OS=$(shell uname)

#METHOD=0# Z3
#METHOD=1# MATHSAT
METHOD=2# SMTINTERPOL

ALLOWED_ATTEMPS=100

THEORY=QF_TO
#THEORY=QF_IDL
#THEORY=QF_UTVPI
#THEORY=QF_LIA

#FILE_TEST=./tests/smt2-files/ijcar_2018_paper_example4_n_4.smt2
#FILE_TEST=./tests/smt2-files/ijcar_2018_paper_example4_n_8.smt2
FILE_TEST=./tests/smt2-files/maxdiff_paper_example.smt2
#FILE_TEST=./tests/smt2-files/maxdiff_paper_example_another_another.smt2
#FILE_TEST=./tests/smt2-files/length_example.smt2

all: tests/one
#all: tests/all
#all: tests/print_all

# ----------------------------------------------------------
#  Rules to build the project
$(LDIR)/libz3.so:
	CURRENT_DIR=$$(pwd); sed -i "s|replace_once|$${CURRENT_DIR}|g" ./include/AXDInterpolant.h
ifeq ($(OS), Darwin)
	cp ./lib/for_mac_libz3.so ./lib/libz3.so
else
ifeq ($(OS), Linux)
	if [ "$(shell printf '%s\n' ${LINUXOLDCOMPVERSION} ${COMPILERVERSION} | sort -V | head -n1)" = "${COMPILERVERSION}" ]; \
	then cp ./lib/for_linux_libz3_old.so ./lib/libz3.so; \
	else cp ./lib/for_linux_libz3.so ./lib/libz3.so; \
	fi
endif
endif

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) $(LDIR)/libz3.so
	@mkdir -p ./obj
	$(CC) -g -c -o $@ $(FLAGS) $<

bin/axd_interpolator: $(OBJS) $(LDIR)/libz3.so
	@mkdir -p ./bin
	$(CC) -g -o $@ $(OBJS) $(FLAGS) -lpthread
# ----------------------------------------------------------

# -------------------------------------------
#  Rules to test a single or many smt2 files
tests/one: bin/axd_interpolator
	./bin/axd_interpolator \
		$(THEORY) $(FILE_TEST) $(METHOD) $(ALLOWED_ATTEMPS)
	rm -rf tests/*.o $@

tests/all: bin/axd_interpolator
	for smt_file in ./tests/smt2-files/*.smt2; do \
		./bin/axd_interpolator \
		$(THEORY) $${smt_file} $(METHOD) $(ALLOWED_ATTEMPS) ; \
		done
	rm -rf tests/*.o $@

tests/print_all: bin/axd_interpolator
	for smt_file in ./tests/smt2-files/*.smt2; do \
		if [ "${METHOD}" = "0" ]; \
		then METHOD_NAME="Z3"; \
		else \
		if [ "${METHOD}" = "1" ]; \
		then METHOD_NAME="MATHSAT"; \
		else METHOD_NAME="DIRECT"; \
		fi \
		fi; \
		./bin/axd_interpolator \
		$(THEORY) $${smt_file} $(METHOD) $(ALLOWED_ATTEMPS) \
		> $${smt_file}_${THEORY}_$${METHOD_NAME}_output.txt ; \
		done
	rm -rf tests/*.o $@
# -------------------------------------------

# -------------------------------------------
#  Check output
check: 
	@make -C ./output

mathsat_check: 
	SMT_SOLVER=MATHSAT make check

z3_check: 
	SMT_SOLVER=Z3 make check
# -------------------------------------------

# ------------------------------
.PHONY: clean
clean:
	CURRENT_DIR=$$(pwd); sed -i "s|$${CURRENT_DIR}|replace_once|g" ./include/AXDInterpolant.h
	rm -rf $(ODIR)/* output/*.smt2
	rm -rf ./tests/smt2-files/*.txt
	cd output && make clean
	rm -rf ./bin/axd_interpolator
	rm -rf ./lib/libz3.so
# ------------------------------
