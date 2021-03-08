# AXDInterpolator

This project implements an interpolation algorithm
for the theory of arrays extended with the diff 
operator equipped with an index theory T_I. 
The approach computes a reduction from the AXDiff(T_I)
theory to the theory of EUF + T_I and 
calls an interpolation engine to process the 
reduced formula. Currently, the software supports 
iZ3 and Mathsat as such engines.

## Building the project

The project requires some z3 dependencies. The
latter can be achieved using the following command from
the root of the project:

```
git submodule update --init --remote dependencies/z3-interp-plus
```

If the user wants to use the MATHSAT SMT solver, the 
implementation requires a soft link inside the ``bin``
directory. For the latter, use the following command:

```
ln -s <mathsat-path> ./bin/mathsat
```

In the root directory, execute the command:

```
make bin/axd_interpolator
```

If the above is successful, then binary file _axd_interpolator_
will be located inside the _bin_ directory.

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
(declare-fun pred (Int) Int)
(declare-fun succ (Int) Int)
(declare-fun neg (Int) Int)
(declare-fun add (Int Int) Int)
(declare-fun length (ArraySort) Int)
```

followed by additional function declarations as needed. 
The rest of the smt2 should specify two assertions. 
The first one encodes the A-part and the second one the B-part. 

## Using the implementation

The _axd_interpolator_ binaries receives 4 arguments:

* The first argument specifies the theory to be used. Currently, the implementation supports the quantifier free fragment of the following theories: total order, integer difference logic, unit-two variable per inequality (UTVPI), and linear arithmetic logic. The user should specify ``QF_TO`` to use the total order logic, ``QF_IDL`` for integer difference logic, ``QF_UTVPI`` for UTVPI, and ``QF_LIA`` for the linear arithmetic logic option.
* The second argument specifies the path of the smt2 file to work with.
* The third argument specifies the engine/method to be used. For the latter there are two options: 0, 1. The option 0 generates a smt2 file with the reduced formula in the theory EUF + T_I and calls ``Z3`` as the interpolating engine. The option 1 behaves the same as option 0, but calls ``MATHSAT`` as the interpolating engine instead.
* The fourth argument specifies a number of attempts, i.e. a number that bounds the number of executions of the main loop of the implementation.

The following is an example of an execution of the _axd_interpolator_ binary:

```
./bin/axd_interpolator QF_TO ./tests/smt2-files/max_diff_paper_example.smt2 1 100
```

### Note:

If the users is using Mac Os, the command requires the following to be
appended:

```
DYLD_LIBRARY_PATH=./lib ./bin/axd_interpolator QF_TO ./tests/smt2-files/max_diff_paper_example.smt2 1 100
```


The binary ``axd_interpolator`` outputs to the standard output any of the following:

* If the formula is unsatisfiable, then it outputs *Unsatisfiable:* followed by the interpolant obtained.
* If the formula is satisfiable, then the implementation outputs either *Satisfiable:* or *Unknown:*. The last option happens when the internal variable ``num_attempts`` reaches 0.
