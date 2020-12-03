#ifndef _AXD_SIG_
#define _AXD_SIG_

#define inSet(element, set) (set.find(element) != set.end())
#define lhs(x) x.arg(0)
#define rhs(x) x.arg(1)
#define func_name(x) x.decl().name().str()
#define sort_name(x) x.decl().range().name().str()

#include <z3++.h>
#include <set>
#include <utility>

struct AXDSignature {

  struct Z3ExprComparator {
    bool operator() (z3::expr const & a, z3::expr const & b) const;
  };

  typedef std::set<z3::expr, Z3ExprComparator> z3_expr_set;

  z3::context & ctx;

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
