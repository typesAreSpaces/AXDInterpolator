#ifndef _INPUT_FORMULA_PARSER_H
#define _INPUT_FORMULA_PARSER_H

#include "AXDInterpolant.h"
#include <sys/stat.h>
#include <unistd.h>

namespace axdinterpolator {

class InputFormulaParser {

private:
  z3::solver input_parser;

  bool exists_file(const char *);
  unsigned part_a_it, part_b_it;
  z3::expr_vector part_a, part_b;

public:
  InputFormulaParser(const char *, const char *, const char *, z3::context &);
  std::string getDecls();
  z3::expr currentPartA();
  z3::expr currentPartB();
  bool next();
};

int run(int, char **);

} // namespace axdinterpolator

#endif
