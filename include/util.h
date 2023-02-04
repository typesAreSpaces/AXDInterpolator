#ifndef _UTIL_H
#define _UTIL_H

#include "z3++.h"
#include <sys/stat.h>
#include <unistd.h>

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

#endif
