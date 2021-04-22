#ifndef _STANDARD_INPUT_
#define _STANDARD_INPUT_

#include "z3++.h"
#define _DEBUG_STDINPUT_ 0

#ifndef NDEBUG
#   define ASSERT(condition, message) \
  do { \
    if (! (condition)) { \
      std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
      << " line " << __LINE__ << ": " << message << std::endl; \
      std::terminate(); \
    } \
  } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

#include <set>
#include <utility>
#include <map>
#include <vector>
#include <tuple>
#include "Preprocess.h"

class StandardInput {

  friend class AXDInterpolant;

  struct DiffMapEntry : public z3::expr_vector {

    z3::expr_vector      lifted_b;
    z3::expr_vector      lifted_diff_k;
    AXDSignature const & sig;

    DiffMapEntry(
        AXDSignature const &, 
        z3::expr const &, 
        z3::expr const &);

    void push(
        z3::expr const &, 
        z3::expr const &,
        z3::expr const &);
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
    AXDSignature const &    sig;

    DiffMap(
        Preprocessor::ArrayVars const &,
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

  class InstantiatedTerms {
    AXDSignature const &      sig;
    AXDSignature::z3_expr_set terms;
    unsigned                  num_of_instantiations;
    unsigned                  num_of_new_index;

    z3::expr_vector 
      new_succs, new_preds, new_minus, 
      new_adds, new_subtracts,
      new_mults, new_divs, 
      new_mods, new_rems;

    void instantiate_QF_IDL();
    void instantiate_QF_UTVPI();
    void instantiate_QF_LIA();

    public:

    InstantiatedTerms(
        AXDSignature const &,
        z3::expr_vector const &);

    AXDSignature::z3_expr_set const & getInstantiatedTerms() const;
    void operator++();
    void add_var(z3::expr const &);
  };

  AXDSignature const & sig;
  DiffMap              diff_map;
  WriteVector          write_vector;
  InstantiatedTerms    instantiated_terms;

  // -) part_1 contains wr-equations and diff(k)-equations
  // of the original input
  // -) part_2 contains the rest
  z3::expr_vector part_1, part_2;

  z3::expr const index_var;
  z3::expr axiom_8, axiom_9;

  z3::expr generateAxiom8(Preprocessor::ArrayVars const &) 
    const;
  z3::expr generateAxiom9() const;

  public:
  StandardInput(
      AXDSignature const &,
      z3::expr_vector const &, 
      z3::expr_vector &,
      Preprocessor::ArrayVars const &);

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
