#ifndef _AXD_SIG_
#define _AXD_SIG_

#include "util.h"

#define FRESH_ARRAY_PREFIX "fresh_array_"
#define FRESH_ELEMENT_PREFIX "fresh_element_"
#define FRESH_INDEX_PREFIX "fresh_index_"
#define FRESH_COMMON_PREFIX "fresh_"
#define DETECT_THEORY 1

#define lhs(x) x.arg(0)
#define rhs(x) x.arg(1)
#define func_name(x) x.decl().name().str()
#define _get_sort(x) x.decl().range()
#define sort_name(x) x.decl().range().name().str()
#define func_kind(x) x.decl().decl_kind()

// Notes:
// The implementation considers
// elements of any index theory
// to have int sort.

namespace axdinterpolator {

struct AXDSignature {

  enum TheoryName { QF_TO, QF_IDL, QF_UTVPI, QF_LIA };

  z3::context &ctx;

  TheoryName theory_name;

  z3::sort const bool_sort, int_sort;

  // "es" stands for elements sorts
  z3_sort_vector_unique element_sorts;
  z3::sort_vector array_sorts;
  z3::expr_vector undefined_es;
  z3::expr_vector el_es;
  z3::expr_vector empty_array_es;
  z3::func_decl_vector diff_es, diff_k_es, wr_es, rd_es, length_es;

  // 1. Given the id() of a type A
  // returns the indexing-position of the abstract type ArraySortA
  // 2. Given the id() of an abstract type ArraySortA
  // returns the indexing-position of the abstract type ArraySortA
  std::map<unsigned, unsigned> array_sort_map;

  AXDSignature(z3::context &, char const *, std::string);

  void processArrayDecls(std::string);
  void indexElementSorts();

  bool is_QF_TO() const;
  bool is_QF_IDL() const;

  void setTheory(TheoryName);
  TheoryName const &getTheoryName() const;

  z3::sort getArraySortBySort(z3::sort const &) const;
  z3::sort getArraySortBySort(unsigned) const;

  z3::expr getUndefinedBySort(z3::sort const &) const;
  z3::expr getUndefinedBySort(unsigned) const;

  z3::expr getEmptyArrayBySort(z3::sort const &) const;
  z3::expr getEmptyArrayBySort(unsigned) const;

  z3::func_decl getDiffBySort(z3::sort const &) const;
  z3::func_decl getDiffBySort(unsigned) const;

  z3::func_decl getDiff_BySort(z3::sort const &) const;
  z3::func_decl getDiff_BySort(unsigned) const;

  z3::func_decl getWrBySort(z3::sort const &) const;
  z3::func_decl getWrBySort(unsigned) const;

  z3::func_decl getRdBySort(z3::sort const &) const;
  z3::func_decl getRdBySort(unsigned) const;

  friend std::ostream &operator<<(std::ostream &, TheoryName const &);
};

class ArrayVars {

  friend class AXDInterpolant;
  friend struct AXDSignature;
  friend class CircularPairIterator;

  // 'Container' is an indexed set of array
  // variables. The index is the id of the
  // 'abstract sort' encoding the type of the
  // array variable
  typedef std::map<unsigned, z3_expr_set> Container;

  Container vars;
  unsigned size;

public:
  ArrayVars();
  void insert(z3::expr const &);
  bool isEmpty() const;
  unsigned getSize() const;
  bool areCommonPairsAvaible() const;

  Container::const_iterator begin() const;
  Container::const_iterator end() const;
};

struct DiffMapEntry : public z3::expr_vector {

  z3::expr_vector lifted_b;
  z3::expr_vector lifted_diff_k;
  AXDSignature const &sig;

  DiffMapEntry(AXDSignature const &, z3::expr const &, z3::expr const &);

  void push(z3::expr const &, z3::expr const &, z3::expr const &);
};

// DiffMap :
// c_array_var.id() x c_array_var.id() -> sequence of diff_k
struct DiffMap {

  friend class SeparatedPair;

  typedef std::pair<z3::expr, z3::expr> z3_expr_pair;

  struct Z3ExprExprComparator {
    bool operator()(z3_expr_pair const &a, z3_expr_pair const &b) const;
  };

  std::map<z3_expr_pair, DiffMapEntry, Z3ExprExprComparator> m_map;
  AXDSignature const &sig;

  DiffMap(ArrayVars const &, AXDSignature const &);

  void add(z3::expr const &, z3::expr const &, z3::expr const &);
  void add_aux(z3::expr const &, z3::expr const &, z3::expr const &);

  unsigned size_of_entry(z3_expr_pair const &);

  z3::expr lift_diff_k(unsigned, z3::expr const &, z3::expr const &) const;

  friend std::ostream &operator<<(std::ostream &, DiffMap const &);
};

// Container keeping track of a, b, i, e from
// a = wr(b, i, e) equations
struct WriteVector {
  std::vector<std::tuple<z3::expr, z3::expr, z3::expr, z3::expr>> m_vector;
  WriteVector();
  void add(z3::expr const &, z3::expr const &, z3::expr const &,
	   z3::expr const &);

  friend std::ostream &operator<<(std::ostream &, WriteVector const &);
};

class InstantiatedTerms {
  AXDSignature const &sig;
  z3_expr_set terms;
  unsigned num_of_instantiations;
  unsigned num_of_new_index;

  z3::expr_vector new_succs, new_preds, new_minus, new_adds, new_subtracts;

  void instantiate_QF_IDL();
  void instantiate_QF_UTVPI();
  void instantiate_QF_LIA();

public:
  InstantiatedTerms(AXDSignature const &, z3::expr_vector const &);

  z3_expr_set const &getInstantiatedTerms() const;
  unsigned getNumOfNewIndex() const;
  void operator++();
  void add_var(z3::expr const &);
};

// The CircularPairIterator
// uses two pointers to the same z3_expr_set
// to check pairs of z3::expr's of the same
// type
class CircularPairIterator {

  // Arrayvars is a data structure
  // using a 'Container' which is a
  // map from sort.id's to z3_expr_set's
  ArrayVars const &vars;
  ArrayVars::Container::const_iterator p_array_vars_it;
  // z3_expr_set is a set of z3::expr's using Z3Exprcomparator
  // as comparator
  z3_expr_set::const_iterator z3_expr_set_first, z3_expr_set_second;
  bool const enableCircularity;

  void avoidLowerDiagonalAndDifferentTypes();

public:
  CircularPairIterator(ArrayVars const &, bool);
  void next();
  bool end();
  DiffMap::z3_expr_pair operator*() const;
};

} // namespace axdinterpolator

#endif
