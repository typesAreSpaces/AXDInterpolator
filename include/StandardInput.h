#ifndef _STANDARD_INPUT_
#define _STANDARD_INPUT_
#include <utility>
#define _DEBUG_STDINPUT_ 1

#include <z3++.h>
#include <map>
#include <vector>
#include <tuple>

class StandardInput {
  class DiffTableEntry {
    public:
      DiffTableEntry();
  };

  class DiffTable {
    std::map<std::pair<z3::expr, z3::expr>, DiffTableEntry> m_table;
    public:
    DiffTable();
  };

  class WriteVector {
    std::vector<std::tuple<z3::expr, z3::expr, z3::expr> > m_vector;
    public:
    WriteVector();
    void add(z3::expr const &, z3::expr const &, z3::expr const &);
  };

  DiffTable diff_table;
  WriteVector write_vector;

  z3::expr_vector part_1, part_2;

  public:
  StandardInput(z3::expr const &);
  void update();
  z3::expr_vector const & getPart_1() const;
  z3::expr_vector const & getPart_2() const;
  z3::expr orientBinPredicate(z3::expr const &);
};

#endif
