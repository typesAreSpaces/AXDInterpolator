#ifndef _STANDARD_INPUT_
#define _STANDARD_INPUT_
#define _DEBUG_STDINPUT_ 0

#include <unordered_set>
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

    std::map<std::pair<unsigned, unsigned>, DiffMapEntry> m_map;
    
    DiffMap(z3::context &, std::unordered_set<unsigned> const &);

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
      std::unordered_set<unsigned> const &,
      AXDSignature const &);
  void initSaturation(); // TODO: implement
  void updateSaturation(); // TODO: implement
};

#endif
