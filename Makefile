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

#METHOD=0 # Z3
METHOD=1 # MATHSAT
#METHOD=2 # DIRECT COMPUTATION

ALLOWED_ATTEMPS=10

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
		./bin/axd_interpolator \
		$(THEORY) $${smt_file} $(METHOD) $(ALLOWED_ATTEMPS) \
		>> $${smt_file}_${THEORY}_output.txt ; \
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
	rm -rf $(ODIR)/* output/*.smt2
	rm -rf ./tests/smt2-files/*.txt
	cd output && make clean
	rm -rf ./bin/axd_interpolator
	rm -rf ./lib/libz3.so
# ------------------------------
