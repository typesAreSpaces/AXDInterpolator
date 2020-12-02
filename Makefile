IDIR = ./include
ODIR = ./obj
SDIR = ./src
LDIR = ./lib
CC = g++
FLAGS = -I$(SDIR) -I$(IDIR) -std=c++11 -Wall

SRC = $(wildcard $(SDIR)/*.cpp)
OBJS = $(SRC:$(SDIR)/%.cpp=$(ODIR)/%.o) $(LDIR)/libz3.so
DEPS = $(wildcard $(IDIR)/*.h)
OS = $(shell uname)

FILE_TEST = ./tests/smt2-files/example.smt2 
#FILE_TEST = ./tests/smt2-files/example1.smt2 
#FILE_TEST = ./tests/smt2-files/example2.smt2 
#FILE_TEST = ./tests/smt2-files/example3.smt2 
#FILE_TEST = ./tests/smt2-files/example4.smt2 
#FILE_TEST = ./tests/smt2-files/example5.smt2 

all: tests/one
#all: tests/all

# ----------------------------------------------------------
#  Rules to build the project

$(LDIR)/libz3.so:
ifeq ($(OS), Darwin)
		cp ./lib/for_mac_libz3.so ./lib/libz3.so
else
ifeq ($(OS), Linux)
		cp ./lib/for_linux_libz3.so ./lib/libz3.so
endif
endif

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) $(LDIR)/libz3.so
	@mkdir -p ./obj
	$(CC) -g -c -o $@ $(FLAGS) $<

bin/axd_interpolator: $(OBJS) $(LDIR)/libz3.so
	@mkdir -p ./bin
	$(CC) -g -o $@ $(OBJS) ./tests/main.cpp $(FLAGS) -lpthread
	
# ----------------------------------------------------------

# -------------------------------------------
#  Rules to test a single or many smt2 files

tests/one: bin/axd_interpolator
	./bin/axd_interpolator $(FILE_TEST)
	rm -rf tests/*.o $@

tests/all: bin/axd_interpolator
	for smt_file in ./tests/smt2-files/*; do \
		./bin/axd_interpolator $${smt_file} > $${smt_file}.txt; \
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
