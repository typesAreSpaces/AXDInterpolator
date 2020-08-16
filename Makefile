Z3DIR = /home/jose/Documents/GithubProjects/z3-interp-plus
Z3EXT = so    # Linux extension
#Z3EXT = dylib # macOS extension

IDIR = ./include
ODIR = ./obj
SDIR = ./src
CC = g++
FLAGS = -I$(SDIR) -I$(IDIR) -std=c++11 -Wall

SRC  = $(wildcard $(SDIR)/*.cpp)
OBJS = $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRC)) $(Z3DIR)/build/libz3.$(Z3EXT) 
DEPS = $(wildcard $(IDIR)/*.h)
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

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	@mkdir -p ./obj
	$(CC) -g -c -o $@ $(FLAGS) $<

bin/axd_interpolator: $(OBJS)
	@mkdir -p ./bin
	$(CC) -g -o $@ $(OBJS) ./tests/main.cpp $(FLAGS) -lpthread
	
# ----------------------------------------------------------
#
# -------------------------------------------
#  Rules to test a single or many smt2 files

tests/one: bin/axd_interpolator
	./bin/axd_interpolator $(FILE_TEST)
	rm -rf tests/*.o $@

tests/all: bin/axd_interpolator
	for smt_file in ./tests/smt2-files/*; \
		do ./bin/axd_interpolator $${smt_file}; \
		done
	rm -rf tests/*.o $@
	
# -------------------------------------------
#
# ------------------------------
.PHONY: clean
clean:
	rm -rf $(ODIR)/* output/*.smt2
	cd output && make clean
	rm -rf /bin/axd_interpolator
# ------------------------------

