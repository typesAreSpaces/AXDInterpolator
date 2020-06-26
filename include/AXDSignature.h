#ifndef _AXD_SIG_
#define _AXD_SIG_

#include <z3++.h>

struct AXDSignature {
  z3::sort 
    int_sort, 
    index_sort, 
    element_sort, 
    array_sort;
  z3::func_decl 
    diff,
    diff_k,
    wr,
    rd;
  AXDSignature(z3::context &);
};

#endif
