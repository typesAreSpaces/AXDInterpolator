#ifndef _AXD_SIG_
#define _AXD_SIG_

#define FRESH_ARRAY_PREFIX   "fresh_array_"
#define FRESH_ELEMENT_PREFIX "fresh_element_"
#define FRESH_INDEX_PREFIX   "fresh_index_"
#define FRESH_COMMON_PREFIX  "fresh_"

//#define m_out std::cerr
#define m_out std::cout

#define inSet(element, set) (set.find(element) != set.end())
#define lhs(x) x.arg(0)
#define rhs(x) x.arg(1)
#define func_name(x) x.decl().name().str()
#define sort_name(x) x.decl().range().name().str()

#include <z3++.h>
#include <set>
#include <utility>

// Notes:
// The implementation considers
// elements of any index theory 
// to have int sort.

struct AXDSignature {

  struct Z3ExprComparator {
    bool operator() (z3::expr const & a, z3::expr const & b) const;
  };

  typedef std::set<z3::expr, Z3ExprComparator> z3_expr_set;

  z3::context & ctx;

  z3::sort const
    int_sort, 
    index_sort, 
    element_sort, 
    array_sort;

  z3::expr const 
    undefined, 
    empty_array;

  z3::func_decl const
    diff, diff_k,
    wr, rd,
    pred, succ, 
    neg, add, 
    length;

  AXDSignature(z3::context &);
  friend std::ostream & operator << (std::ostream &, z3_expr_set const &);
};

#endif
