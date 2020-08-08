Z3_SMT_FILES=$(wildcard *z3.smt2)
Z3_OUT_FILES=$(patsubst %z3.smt2, %z3_interpolant.txt, $(Z3_SMT_FILES))

MATHSAT_SMT_FILES=$(wildcard *mathsat.smt2)
MATHSAT_OUT_FILES=$(patsubst %mathsat.smt2, %mathsat_interpolant.txt, $(MATHSAT_SMT_FILES))

SOLVER=$(MATHSAT_OUT_FILES)

all: $(SOLVER) 

%mathsat_interpolant.txt: %mathsat.smt2
	mathsat $^ > $@

%z3_interpolant.txt: %z3.smt2
	z3 $^ > $@

.PHONY: clean
clean:
	rm -rf *.txt
