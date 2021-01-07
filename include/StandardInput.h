#ifndef _STANDARD_INPUT_
#define _STANDARD_INPUT_

#include "z3++.h"
#define _DEBUG_STDINPUT_ 0
#define _CHECK_PREPROCESS_INV_ 0

#include <set>
#include <utility>
#include <map>
#include <vector>
#include <tuple>
#include "AXDSignature.h"

class StandardInput : public AXDSignature {

  friend class AXDInterpolant;
  // DiffMap : 
  // c_array_var.id() x c_array_var.id() -> sequence of diff_k
  struct DiffMap {

    typedef std::pair<z3::expr, z3::expr> z3_expr_pair;
    struct Z3ExprExprComparator {
      bool operator() (
          z3_expr_pair const & a, 
          z3_expr_pair const & b) const;
    };

    std::map<z3_expr_pair, 
      z3::expr_vector, 
      Z3ExprExprComparator> m_map;

    DiffMap(z3::context &, z3_expr_set const &);

    void add(
        z3::expr const &, 
        z3::expr const &, 
        z3::expr const &);
    void add_aux(
        z3::expr const &, 
        z3::expr const &, 
        z3::expr const &);
    unsigned size_of_entry(z3_expr_pair const &);
  };

  // Container keeping track of a, b, i, e from 
  // a = wr(b, i, e) equations
  struct WriteVector {
    std::vector<
      std::tuple<
      z3::expr, 
      z3::expr, 
      z3::expr, 
      z3::expr> 
        > m_vector;
    WriteVector();
    void add(
        z3::expr const &, 
        z3::expr const &, 
        z3::expr const &, 
        z3::expr const &);
  };

  DiffMap     diff_map;
  WriteVector write_vector;

  z3::func_decl_vector local_signature;

  // -) part_1 contains wr-equations and diff(k)-equations
  // of the original input
  // -) part_2 contains the rest
  z3::expr_vector part_1, part_2;
  z3::expr_vector index_vars;
  unsigned N_instantiation;
  z3::expr_vector current_instantiated_index_terms;
  z3::expr index_var, axiom_8, axiom_9;

  z3::expr orientBinPredicate(z3::expr const &);
  void     N_instantiate();
  void     unaryInstantiationExtension(z3::func_decl const &);
  void     binaryInstantiationExtension(z3::func_decl const &);

  public:
  StandardInput(
      z3::expr_vector const &, 
      z3::expr_vector &,
      z3_expr_set const &,
      char const *);

  void initSaturation(); 
  void updateSaturation(
      DiffMap::z3_expr_pair const &, 
      z3::expr const &, 
      unsigned min_dim); 
  void instantiate(z3::solver &, z3::expr &) const;
  void instantiate(std::ostream &, z3::expr &) const;
  void instantiate(z3::expr_vector &, z3::expr &) const;

  friend std::ostream & operator << (std::ostream &, 
      StandardInput const &);
  friend std::ostream & operator << (std::ostream &,
      StandardInput::DiffMap const &);
  friend std::ostream & operator << (std::ostream &,
      StandardInput::WriteVector const &);
};

#endif
