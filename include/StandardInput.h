#ifndef _STANDARD_INPUT_
#define _STANDARD_INPUT_
#include <unordered_set>
#include <utility>
#define _DEBUG_STDINPUT_ 0

#include <z3++.h>
#include <map>
#include <vector>
#include <tuple>

class StandardInput {
  struct DiffTableEntry {
    z3::expr_vector index;
    DiffTableEntry(z3::context &);
  };

  struct DiffTable {
    std::map<std::pair<unsigned, unsigned>, DiffTableEntry> m_table;
    DiffTable(std::unordered_set<unsigned> const &, z3::context &);
    void add(z3::expr const &, z3::expr const &, z3::expr const &);
    void add_aux(z3::expr const &, z3::expr const &, z3::expr const &);
  };

  struct WriteVector {
    std::vector<std::tuple<z3::expr, z3::expr, z3::expr> > m_vector;
    WriteVector();
    void add(z3::expr const &, z3::expr const &, z3::expr const &);
  };

  z3::expr_vector part_1, part_2;
  std::unordered_set<unsigned> const & array_var_ids;

  DiffTable diff_table;
  WriteVector write_vector;

  public:
  StandardInput(z3::expr const &, std::unordered_set<unsigned> const &);
  void update();
  z3::expr_vector const & getPart_1() const;
  z3::expr_vector const & getPart_2() const;
  z3::expr orientBinPredicate(z3::expr const &);
};

#endif
