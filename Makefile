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

# --------------------------------
#  Build

all: tests/smt_file
#all: tests/all

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	@mkdir -p ./obj
	$(CC) -g -c -o $@ $(FLAGS) $<
	
# --------------------------------
#
# ----------------------------------------------------------
#  Tests

tests/smt_file: $(OBJS)
	@mkdir -p output
	$(CC) -g -o $@ $(OBJS) ./tests/main.cpp $(FLAGS) -lpthread
	./$@ $(FILE_TEST)
	rm -rf tests/*.o $@

tests/all: $(OBJS) 
	@mkdir -p output
	$(CC) -g -o $@ $(OBJS) ./tests/main.cpp $(FLAGS) -lpthread
	for smt_file in ./tests/smt2-files/*; \
		do ./$@ $${smt_file}; \
		done
	rm -rf tests/*.o $@
	
# ----------------------------------------------------------
#
# ------------------------------
.PHONY: clean
clean:
	rm -rf $(ODIR)/* output/*.smt2
	cd output && make clean
# ------------------------------

