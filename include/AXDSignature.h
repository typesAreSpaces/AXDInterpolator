#ifndef _AXD_SIG_
#define _AXD_SIG_

#define FRESH_ARRAY_PREFIX   "fresh_array_"
#define FRESH_ELEMENT_PREFIX "fresh_element_"
#define FRESH_INDEX_PREFIX   "fresh_index_"
#define FRESH_COMMON_PREFIX  "fresh_"

//#define m_out std::cerr
#define m_out std::cout

#define inSet(element, set) (set.find(element) != set.end())
#define lhs(x)              x.arg(0)
#define rhs(x)              x.arg(1)
#define func_name(x)        x.decl().name().str()
#define sort_name(x)        x.decl().range().name().str()

#include <z3++.h>
#include <set>
#include <cstring>
#include <utility>

// Notes:
// The implementation considers
// elements of any index theory 
// to have int sort.

struct AXDSignature {

  enum TheoryName { QF_TO, QF_IDL, QF_UTVPI, QF_LIA };

  struct Z3ExprComparator {
    bool operator() (z3::expr const & a, z3::expr const & b) const;
  };

  typedef std::set<z3::expr, Z3ExprComparator> z3_expr_set;

  z3::context & ctx;
  TheoryName    theory_name;

  z3::sort const
    bool_sort, 
    int_sort, 

    // -------------------------------
    // [TODO] remove the following
    // Parametrize them instead with
    // every type A from (Array Int A)
    element_sort,
    array_sort;

  z3::expr const 
    undefined, 
    empty_array;

  z3::func_decl const
    diff, diff_k,
    wr, rd,
    length;
  // -------------------------------

  AXDSignature(z3::context &, char const *);

  bool is_QF_TO() const;
  bool is_QF_IDL() const;
  TheoryName getTheoryName() const;
  friend std::ostream & operator << (std::ostream &, z3_expr_set const &);
};

#endif
