Z3_SMT_FILES=$(wildcard *z3.smt2)
MATHSAT_SMT_FILES=$(wildcard *mathsat.smt2)

ifeq ($(SMT_SOLVER), MATHSAT)
	TEST=$(MATHSAT_SMT_FILES:%mathsat.smt2=%mathsat_interpolant.txt)
else
ifeq ($(SMT_SOLVER), Z3)
	TEST=$(Z3_SMT_FILES:%z3.smt2=%z3_interpolant.txt)
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

%mathsat_interpolant.txt: %mathsat.smt2
	mathsat $^ > $@

%z3_interpolant.txt: %z3.smt2
	z3 $^ > $@

.PHONY: clean
clean:
	rm -rf *.txt
