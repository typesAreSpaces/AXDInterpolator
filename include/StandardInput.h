#ifndef _STANDARD_INPUT_
#define _STANDARD_INPUT_

#include "z3++.h"
#define _DEBUG_STDINPUT_ 0

#include <set>
#include <utility>
#include <map>
#include <vector>
#include <tuple>
#include "AXDSignature.h"

class StandardInput {

  AXDSignature const & sig;

  unsigned s_fresh_index;

  friend class AXDInterpolant;

  struct DiffMapEntry : public z3::expr_vector {

    z3::expr_vector lifted_b;
    z3::expr_vector lifted_diff_k;
    AXDSignature const & sig;

    DiffMapEntry(
        z3::context &, 
        AXDSignature const &, 
        z3::expr const &, 
        z3::expr const &);

    void push(
        z3::expr const &, 
        z3::expr const &,
        z3::expr const &
        );
  };

  // DiffMap : 
  // c_array_var.id() x c_array_var.id() -> sequence of diff_k
  struct DiffMap {

    friend class StandardInput;

    typedef std::pair<z3::expr, z3::expr> z3_expr_pair;
    struct Z3ExprExprComparator {
      bool operator() (
          z3_expr_pair const & a, 
          z3_expr_pair const & b) const;
    };

    std::map<z3_expr_pair, 
      DiffMapEntry,
      Z3ExprExprComparator> m_map;
    AXDSignature const & sig;

    DiffMap(z3::context &, 
        AXDSignature::z3_expr_set const &,
        AXDSignature const &);

    void add(
        z3::expr const &, 
        z3::expr const &, 
        z3::expr const &);
    void add_aux(
        z3::expr const &, 
        z3::expr const &, 
        z3::expr const &);

    unsigned size_of_entry(z3_expr_pair const &);

    z3::expr lift_diff_k(
        unsigned,
        z3::expr const &,
        z3::expr const &
        ) const;
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

  void     N_instantiate();
  void     unaryInstantiationExtension(z3::func_decl const &);
  void     binaryInstantiationExtension(z3::func_decl const &);

  z3::expr fresh_index_constant();
  z3::expr fresh_element_constant();

  public:
  StandardInput(
      AXDSignature const &,
      z3::expr_vector const &, 
      z3::expr_vector &,
      AXDSignature::z3_expr_set const &,
      char const *, unsigned);

  void initSaturation(); 
  void updateSaturation(
      DiffMap::z3_expr_pair const &, 
      z3::expr const &, 
      unsigned min_dim); 
  void instantiate(z3::solver &, z3::expr &) const;
  void instantiate(std::ostream &, z3::expr &) const;
  void instantiate(z3::expr_vector &, z3::expr &) const;

  unsigned get_fresh_index() const;

  friend std::ostream & operator << (std::ostream &, 
      StandardInput const &);
  friend std::ostream & operator << (std::ostream &,
      StandardInput::DiffMap const &);
  friend std::ostream & operator << (std::ostream &,
      StandardInput::WriteVector const &);
};

#endif
