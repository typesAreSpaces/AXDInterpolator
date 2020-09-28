# AXDInterpolator

This project implements an interpolation algorithm
for the theory of arrays extended with the diff 
operator by computing a reduction from the AXDiff
theory to the theory of EUF with total order and 
calling an interpolation engine to process the 
reduced formula. Currently, the software supports 
iZ3 and Mathsat as such engines.

## Setting up the input formula

Input formulas are encoded as a smt2 file 
in the ./tests/smt2-files directory. Such smt2 
file should include the next sort and function 
declarations:

```
; Sorts
(declare-sort ElementSort 0)
(declare-sort ArraySort 0)

; Function declarartions
(declare-fun diff (ArraySort ArraySort) Int)
(declare-fun wr (ArraySort Int ElementSort) ArraySort)
(declare-fun rd (ArraySort Int) ElementSort)
```

followed by additional function declarations as needed. 
The rest of the smt2 should specify two assertions. 
The first one encodes the A-part and the second one the B-part. 
Additionally the location of the smt2 file should be 
edited in the Makefile under the variable ``FILE_TEST``.

## Building the project

The project supports three workflows to compute interpolants:

### Computing interpolants directly

#### Requirements: 
- Set ``_DIRECT_INTERP_COMPUTATION_`` to 1
in the header file ./include/AXDInterpolant.h. 

#### Workflow:
1. Execute the ``make`` command from
terminal to produce a file in the ./output 
directory containing the interpolant.

#### Notes: 
- The file is named by suffixing 
`_interpolant` to the original name of the 
query file. 

### Computing interpolants using iZ3 

#### Requirements: 
- Set ``_Z3_OUTPUT_FILE_`` to 1
in the header file ./include/AXDInterpolant.h. 

#### Workflow:
1. Execute the ``make`` command from
terminal to produce a smt2 file in the ./output 
directory with the reduced formulas
in the theory of EUF with total order. 

2. Execute the z3 binary on the file produced 
from the bin folder 
at the root of the project.

#### Notes: 
- The file is named by suffixing 
`_reduced_z3` to the original name of the 
query file. 


### Computing interpolants using Mathsat

#### Requirements: 
- Set ``_MATHSAT5_OUTPUT_FILE_`` to 1
in the header file ./include/AXDInterpolant.h. 

#### Workflow:
1. Execute the ``make`` command from 
terminal to produce a smt2 file in the ./output 
directory with the reduced formulas
in the theory of EUF with total order.

2. Execute the mathsat binary on the file produced
from the bin folder 
at the root of the project.

#### Notes: 
- The file is named by suffixing 
`_reduced_mathsat` to the original name of the 
query file. 
