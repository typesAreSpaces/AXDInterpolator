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

# ------------------------------------------------------------------------------------------
#  Build

all: tests/basic

obj:
	mkdir -p obj

output:
	mkdir -p output

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) obj output
	$(CC) -g -c -o $@ $(FLAGS) $<

# ------------------------------------------------------------------------------------------
#  Tests

.PHONY: tests/basic
tests/basic: $(OBJS)
	$(CC) -g -c -o ./$@.o $(FLAGS) ./$@.cpp
	$(CC) -g -o $@ $(OBJS) ./$@.o -lpthread -Wall
	#./$@ $(FILE_TEST)
	#./$@ ./tests/smt2-files/example.smt2 
	#./$@ ./tests/smt2-files/example1.smt2 
	#./$@ ./tests/smt2-files/example2.smt2 
	./$@ ./tests/smt2-files/example3.smt2 
	rm -rf tests/*.o $@

# ------------------------------------------------------------------------------------------

.PHONY: clean
clean:
	rm -rf $(ODIR)/* output/*
