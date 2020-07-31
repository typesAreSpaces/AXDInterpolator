#ifndef _STANDARD_INPUT_
#define _STANDARD_INPUT_
#define _DEBUG_STDINPUT_ 0

//#include <unordered_set>
#include <set>
#include <utility>
#include <map>
#include <vector>
#include <tuple>
#include "AXDSignature.h"

class StandardInput {

  friend class AXDInterpolant;

  // DiffMap : c_array_var.id() x c_array_var.id() -> sequence of diff_k
  struct DiffMap {

    struct DiffMapEntry {
      z3::expr_vector new_index_vars;
      DiffMapEntry(z3::context &);
    };
    
    struct Z3ExprExprComparator {
      bool operator() (std::pair<z3::expr, z3::expr> const & a, std::pair<z3::expr, z3::expr> const & b){
        return a.first.id() > b.first.id() || (a.first.id() == b.first.id() && a.second.id() > b.second.id());
      }
    };

    std::map<std::pair<z3::expr, z3::expr>, 
      DiffMapEntry, 
      Z3ExprExprComparator> m_map;
    
    DiffMap(z3::context &, z3_expr_set const &);

    void add(z3::expr const &, z3::expr const &, z3::expr const &);
    void add_aux(z3::expr const &, z3::expr const &, z3::expr const &);
  };

  // Container keeping track of a, b, i, e from 
  // a = wr(b, i, e) equations
  struct WriteVector {
    std::vector<std::tuple<z3::expr, z3::expr, z3::expr, z3::expr> > m_vector;
    WriteVector();
    void add(z3::expr const &, z3::expr const &, z3::expr const &, z3::expr const &);
  };

  AXDSignature const & sig;
  DiffMap diff_map;
  WriteVector write_vector;

  // -) part_1 contains wr-equations and diff(k)-equations
  // of the original input
  // -) part_2 contains the rest
  z3::expr_vector part_1, part_2;
  z3::expr_vector const & initial_index_vars;

  z3::expr orientBinPredicate(z3::expr const &);

  public:
  StandardInput(
      z3::expr const &, 
      z3::expr_vector const & initial_index_vars,
      z3_expr_set const &,
      AXDSignature const &);
  void initSaturation(); // TODO: implement
  void updateSaturation(); // TODO: implement
};

#endif
