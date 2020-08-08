# AXDInterpolator

This project implements an interpolation algorithm for the theory of
arrays extended with the diff operator by computing a reduction from the AXDiff
theory to the theory of EUF with total order and calling an interpolation engine
to process the reduced formula. Currently, the software supports Z3 and Mathsat 
as such engines.

## Building the project

The project requires the following clone of Z3, which is the Z3 SMT solver
containing interpolantion procedures and additional features required for
the AXDInterpolator project: 
https://github.com/typesAreSpaces/z3-interp-plus

The instructions for the installation of the Z3 clone are identical to 
the original Z3 SMT solver.

Once z3-interp-plus is installed, it is necessary to modify the Makefile in the root
of the project by changing the Z3DIR variable with the location of installation of z3-interp-plus
in the local machine.

In order to compute an interpolant is necessary to write the query as an smt2 file 
in the ./tests/smt2-files directory. Such smt2 file should include the next 
sort/function declarations

```
; Sorts
(declare-sort ElementSort 0)
(declare-sort ArraySort 0)

; Function declarartions
(declare-fun diff (ArraySort ArraySort) Int)
(declare-fun wr (ArraySort Int ElementSort) ArraySort)
(declare-fun rd (ArraySort Int) ElementSort)
```

followed by additional function declarations as needed. The rest of the
smt2 should specify two assertions. The first one encodes the
A-part and the second one the B-part. Additionally the location of the smt2 file
should be edited in the Makefile under the variable ``FILE_TEST``.

There are three workflows to compute interpolants with the project:

### Computing interpolants directly
This requires to set the macro ``_DIRECT_INTERP_COMPUTATION_`` to 1
in the header file ./include/AXDInterpolant.h. Then a call to ``make`` from the
terminal produces a file in the ./output directory containing the interpolant.
The file is named by suffixing `_interpolant` to the original name of the query file. 

### Computing interpolants using an interface to Z3
This requires to set the macro ``_Z3_OUTPUT_FILE_`` to 1
in the header file ./include/AXDInterpolant.h. Then a call to ``make`` from the
terminal produces a smt2 file in the ./output directory with the reduced formulas
in the theory of EUF with total order and the commands for z3 to compute the interpolant.
The file is named by suffixing `_reduced_z3` to the original name of the query file. 


### Computing interpolants using an interface to Mathsat
This requires to set the macro ``_MATHSAT5_OUTPUT_FILE_`` to 1
in the header file ./include/AXDInterpolant.h. Then a call to ``make`` from the
terminal produces a smt2 file in the ./output directory with the reduced formulas
in the theory of EUF with total order and the commands for mathsat to compute the interpolant.
The file is named by suffixing `_reduced_mathsat` to the original name of the query file. 
