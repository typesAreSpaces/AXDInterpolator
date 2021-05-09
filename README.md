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

Input formulas are encoded as a SMTLIB2 file 
in the ./tests/smt2-files directory. If the input
includes function applications of our extended
language, i.e. diff, length, empty_array, undefined,
then the latter elements should be appended a string
denoting the type of the range of the associated
array element. The latter ensures compatibility with
the internal language used in the implementation.
The following are examples of declarations following
the above convention:

```
(declare-fun diffInt ((Array Int Int) (Array Int Int)) Int)
(declare-fun lengthArrayIntInt ((Array Int (Array Int Int))) Int)
(declare-fun empty_arrayArrayIntInt () (Array Int (Array Int Int))
```

The rest of the SMTLIB2 file should specify two assertions. 
The first one encodes the A-part and the second one the B-part. 

## Using the implementation

The _axd_interpolator_ binaries receives 4 arguments:

* The first argument specifies the theory to be used. Currently, the implementation supports the quantifier free fragment of the following theories: total order, integer difference logic, unit-two variable per inequality (UTVPI), and linear arithmetic logic. The user should specify ``QF_TO`` to use the total order logic, ``QF_IDL`` for integer difference logic, ``QF_UTVPI`` for UTVPI, and ``QF_LIA`` for the linear arithmetic logic option.
* The second argument specifies the path of the smt2 file to work with.
* The third argument specifies the engine/method to be used. For the latter there are three options: 0, 1, and 2. 
  - Option 0 uses ``Z3`` 
  - Option 1 uses ``MATHSAT``
  - Option 2 uses ``SMTINTERPOL``
* The fourth argument specifies a number of attempts, i.e. a number that bounds the number of executions of the main loop of the implementation.

The following is an example of an execution of the _axd_interpolator_ binary:

```
./bin/axd_interpolator QF_TO ./tests/smt2-files/maxdiff_paper_example.smt2 1 100
```

### Note:

If Mac Os is used, the command requires the following to be
appended:

```
DYLD_LIBRARY_PATH=./lib ./bin/axd_interpolator QF_TO ./tests/smt2-files/maxdiff_paper_example.smt2 1 100
```


The binary ``axd_interpolator`` outputs to the standard output any of the following:

* If the formula is unsatisfiable, then it outputs *Unsatisfiable:* followed by the interpolant obtained.
* If the formula is satisfiable, then the implementation outputs either *Satisfiable:* or *Unknown:*. The last option happens when the internal variable ``num_attempts`` reaches 0.

## Replicating SMT 2021 submission results

We included a script to obtain the tables reported in our submission for SMT 2021.
For the latter, the following items are neccessary:

* Softlink to a mathsat binary file as indicated above.
* Git with LFS support, git-lfs, or download the `files.zip` raw file from https://github.com/typesAreSpaces/AXDInterpolator/blob/master/tests/verification-files/files.zip. If the user downloads `files.zip` directly, the zip file should be moved to the tests/verification-files directory.

The steps to execute the benchmarking script are the following:

1. Unzip the verification files in tests/verification-files with `unzip files.zip`
2. `cd` into tests/benchmark/scripts and execute the script `./SMT-2021-submission-results.sh`
3. The process will conclude with a Latex-compatible table as output.
