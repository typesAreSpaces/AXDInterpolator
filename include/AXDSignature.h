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
#define func_kind(x)        x.decl().decl_kind()

#include <z3++.h>
#include <set>
#include <cstring>
#include <utility>
#include <regex>
#include <map>
#include <algorithm>

// Notes:
// The implementation considers
// elements of any index theory 
// to have int sort.

struct AXDSignature {

  enum TheoryName { QF_TO, QF_IDL, QF_UTVPI, QF_LIA };

  struct Z3ExprComparator {
    bool operator() (
        z3::expr const & a, 
        z3::expr const & b) const;
  };

  typedef std::set<z3::expr, Z3ExprComparator> z3_expr_set;

  class z3_expr_vector_unique : public z3::expr_vector {
    std::set<unsigned> expr_ids;

    public:
    z3_expr_vector_unique(z3::context &);
    void push(z3::expr const &);
  };

  class z3_sort_vector_unique : public z3::sort_vector {
    std::set<unsigned> sort_ids;

    public:
    z3_sort_vector_unique(z3::context &);
    void push(z3::sort const &);
  };

  static bool isSpaceOrParen(char);
  void        extractNameFromSort(std::string &) const;
  void        processArrayDecls(std::string &);
  void        indexElementSorts();

  bool is_QF_TO() const;
  bool is_QF_IDL() const;

  TheoryName const & getTheoryName() const;
  z3::sort           getArraySortBySort(z3::sort const &) const;
  z3::expr           getUndefinedBySort(z3::sort const &) const;
  z3::expr           getEmptyArrayBySort(z3::sort const &) const;
  z3::func_decl      getDiffBySort(z3::sort const &) const;
  z3::func_decl      getDiff_BySort(z3::sort const &) const;
  z3::func_decl      getWrBySort(z3::sort const &) const;
  z3::func_decl      getRdBySort(z3::sort const &) const;

  friend std::ostream & operator << (
      std::ostream &, z3_expr_set const &);

  z3::context & ctx;
  TheoryName    theory_name;

  z3::sort const
    bool_sort, 
    int_sort;

  // -------------------------------
  // [TODO] remove the following
  // Parametrize them instead with
  // every type A from (Array Int A)
  z3::sort const
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

  // "es" stands for elements sorts
  z3_sort_vector_unique element_sorts;
  z3::sort_vector array_sorts;
  z3::expr_vector undefined_es;
  z3::expr_vector empty_array_es;
  z3::func_decl_vector 
    diff_es,
    diff_k_es,
    wr_es, rd_es,
    length_es;

  // Given the id() of a type A
  // returns the abstract type ArraySortA
  std::map<unsigned, unsigned> arraySortMap;

  AXDSignature(z3::context &, char const *, std::string &);
};

#endif
