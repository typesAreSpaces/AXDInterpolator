#include "util.h"
#include "z3++.h"

InputFormulaParser::InputFormulaParser(const char *theory,
                                       const char *smt_filename,
                                       const char *smt_engine_code,
				       z3::context &ctx)
  : input_parser(ctx), part_a_it(0), part_b_it(0), part_a(ctx), part_b(ctx) {

  if (!exists_file(smt_filename)) {
    std::cout << "File not found." << std::endl;
    // return 1;
    return;
  }

  auto const &to_cnf_tactic = z3::tactic(ctx, "tseitin-cnf");
  auto const &to_dnf_tactic =
      z3::repeat(z3::tactic(ctx, "split-clause") | z3::tactic(ctx, "skip"));
  auto const &my_tactic = to_cnf_tactic & to_dnf_tactic;

  input_parser.from_file(smt_filename);

  auto const &input_formula = input_parser.assertions();
  if (input_formula.size() == 0) {
    std::cout << "No assertions in input file" << std::endl;
    // return 2;
  }

  // Check that indeed we have a pair
  assert(input_formula.size() == 2);

  z3::goal goal_a(ctx);
  z3::goal goal_b(ctx);
  goal_a.add(input_formula[0]);
  goal_b.add(input_formula[1]);

  auto const & _part_a = my_tactic(goal_a);
  for(unsigned i = 0; i < _part_a.size(); i++)
    part_a.push_back(_part_a[i].as_expr());

  auto const & _part_b = my_tactic(goal_b);
  for(unsigned i = 0; i < _part_b.size(); i++)
    part_b.push_back(_part_b[i].as_expr());
}

bool InputFormulaParser::exists_file(const char *name) {
  struct stat buffer;
  return (stat(name, &buffer) == 0);
}

std::string InputFormulaParser::getDecls() {
  return input_parser.to_smt2_decls_only();
}

z3::expr InputFormulaParser::currentPartA() {
  return part_a[part_a_it];
}

z3::expr InputFormulaParser::currentPartB() {
  return part_b[part_b_it];
}

bool InputFormulaParser::next() {

  part_b_it++;

  if (part_b_it >= part_b.size()) {
    part_b_it = 0;
    part_a_it++;
  }

  if (part_a_it >= part_a.size()) {
    part_a_it = 0;
    return false;
  }

  return true;
}
