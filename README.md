# CAXDInterpolator

This project implements an interpolation algorithm
for the theory of arrays extended with the diff 
operator equipped with an index theory $T_I$. 
The approach computes a reduction from the $CAXD(T_I)$
theory to the theory of $EUF \cup T_I$ and 
calls an interpolation engine to process the 
reduced formula. Currently, the software supports the following
interpolation engines:

- [`iZ3`](https://github.com/Z3Prover/z3/releases/tag/z3-4.7.1)
- [`MathSAT`](https://mathsat.fbk.eu)
- [`SMTInterpol`](https://github.com/ultimate-pa/smtinterpol)

## Building the project

In the root directory, execute the commands:

```
./make_common.sh
make -j<N>
```
where `<N>` is the number of cores the build script is allowed to use.

If the above is successful, then the binary file ``caxd_interpolator``
will be located inside the _bin_ directory.

If the user wants to use the `MathSAT` SMT solver, the 
implementation requires a soft link inside the ``bin``
directory. For the latter, use the following command:

```
ln -s <mathsat-path> ./bin/mathsat
```

## Setting up an input formula

We use the type `Int` to encode elements of the Index type. 
The SMT terms `select, store` are used to encode the terms `rd, wr` (read and write) 
respectively.

Input is processed as SMTLIB2 files. If the input
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

The ``caxd_interpolator`` binaries receives 3 arguments:

* The first argument specifies the theory to be used. Currently, the implementation supports the quantifier free fragment of the following theories: total order, integer difference logic, unit-two variable per inequality ($UTVPI$), and linear arithmetic logic. The user should specify $QF_{TO}$ to use the total order logic, $QF_{IDL}$ for integer difference logic, $QF_{UTVPI}$ for $UTVPI$, and $QF_{LIA}$ for the linear arithmetic logic option.
* The second argument specifies the path of the smt2 file to work with.
* The third argument specifies the engine/method to be used. For the latter there are three options: 0, 1, and 2. 
  - Option 0 uses ``iZ3`` 
  - Option 1 uses ``MathSAT``
  - Option 2 uses ``SMTInterpol``

The following is an example of an execution of the ``caxd_interpolator`` binary using ``iZ3`` as interpolating engine:

```
./bin/caxd_interpolator QF_TO ./tests/smt2-files/maxdiff_paper_example.smt2 0
```

### Note:

macOS users need to add ``DYLD_LIBRARY_PATH=./lib`` to execute the binary, i.e.:

```
DYLD_LIBRARY_PATH=./lib ./bin/caxd_interpolator QF_TO ./tests/smt2-files/maxdiff_paper_example.smt2 1 
```


The binary ``caxd_interpolator`` outputs to the standard output any of the following:

* If the formula is unsatisfiable, then it outputs *Unsatisfiable:* followed by the interpolant obtained.
* If the formula is satisfiable, then the implementation outputs *Satisfiable*.

## Replicating submission results

We include a script to produce the tables reported in our submission. 
For the latter, the following items are neccessary:

* Python3
* Softlink to a mathsat binary file as indicated above.
* Git with LFS support, git-lfs, or download the `files-full.zip` raw file from https://github.com/typesAreSpaces/AXDInterpolator/blob/master/tests/smt2-files/benchmark/files-full.zip. If the user downloads `files-full.zip` directly, the zip file should be moved to the `tests/smt2-files/benchmark` directory.

The steps to execute the benchmarking script are the following:

1. Unzip the verification files in `tests/smt2-files/benchmark`  with `unzip files-full.zip`
2. `cd` into `tests/benchmark/scripts` and execute the script `./submission-results.sh`
3. The previous step will write two files `benchmark_memsafety_results.txt` and `benchmark_reachsafety_results.txt` in the `tests/benchmark` directory. 
4. To produce a LaTeX-compatible table with the obtained results, `cd` into `tests/benchmark/scripts` and execute the script `./get_table.sh`. 
