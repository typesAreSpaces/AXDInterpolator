Z3_SMT_FILES=$(wildcard *reduced_z3.smt2)
MATHSAT_SMT_FILES=$(wildcard *reduced_mathsat.smt2)

ifeq ($(SMT_SOLVER), MATHSAT)
	TEST=$(MATHSAT_SMT_FILES:%reduced_mathsat.smt2=%reduced_interpolant_mathsat.txt)
else
ifeq ($(SMT_SOLVER), Z3)
	TEST=$(Z3_SMT_FILES:%reduced_z3.smt2=%reduced_interpolant_z3.txt)
else
	TEST=undef
endif
endif

all: $(TEST)
ifeq ($(TEST), $(MATHSAT_OUT_FILES))
	@echo "Using Mathsat"
endif
ifeq ($(TEST), $(Z3_OUT_FILES))
	@echo "Using Z3"
endif

undef:
	@echo "The smt-solver $(SMT_SOLVER) is not supported"

%reduced_interpolant_mathsat.txt: %reduced_mathsat.smt2
	mathsat $^ > $@

%reduced_interpolant_z3.txt: %reduced_z3.smt2
	z3 $^ > $@

.PHONY: clean
clean:
	rm -rf *.txt
