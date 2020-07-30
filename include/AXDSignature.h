#ifndef _AXD_SIG_
#define _AXD_SIG_

#define inSet(element, set) (set.find(element) != set.end())

#include <z3++.h>

struct AXDSignature {
  z3::context ctx;
  z3::sort 
    int_sort, 
    index_sort, 
    element_sort, 
    array_sort;
  z3::func_decl 
    diff,
    wr,
    rd;
  AXDSignature();
};

#endif
