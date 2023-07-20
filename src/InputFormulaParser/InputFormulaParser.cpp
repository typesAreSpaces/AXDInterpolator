#include "InputFormulaParser.h"
#include "util.h"
#include "z3++.h"

axdinterpolator::InputFormulaParser::InputFormulaParser(
    const char *theory, const char *smt_filename, const char *smt_engine_code,
    z3::context &ctx)
    : input_parser(ctx), part_a_it(0), part_b_it(0), part_a(ctx), part_b(ctx) {

  if (!exists_file(smt_filename)) {
    m_out << "File not found." << std::endl;
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
    m_out << "No assertions in input file" << std::endl;
    // return 2;
  }

  // Check that indeed we have a pair
  assert(input_formula.size() == 2);

  z3::goal goal_a(ctx);
  z3::goal goal_b(ctx);
  goal_a.add(input_formula[0]);
  goal_b.add(input_formula[1]);

  auto const &_part_a = my_tactic(goal_a);
  for (unsigned i = 0; i < _part_a.size(); i++)
    part_a.push_back(_part_a[i].as_expr());

  auto const &_part_b = my_tactic(goal_b);
  for (unsigned i = 0; i < _part_b.size(); i++)
    part_b.push_back(_part_b[i].as_expr());
}

bool axdinterpolator::InputFormulaParser::exists_file(const char *name) {
  struct stat buffer;
  return (stat(name, &buffer) == 0);
}

std::string axdinterpolator::InputFormulaParser::getDecls() {
  return input_parser.to_smt2_decls();
}

z3::expr axdinterpolator::InputFormulaParser::partA() {
  return part_a[part_a_it];
}

z3::expr axdinterpolator::InputFormulaParser::partB() {
  return part_b[part_b_it];
}

bool axdinterpolator::InputFormulaParser::next() {

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

int axdinterpolator::run(int argc, char **argv) {
#if _ENABLE_UNSAT_CORES
  z3::set_param("unsat_core", true);
#endif
#if _COMPUTE_INTERPOLANT_DIRECTLY 
  z3::set_param("proof", true);
#endif
  z3::context ctx;
  ctx.set(":pp-min-alias-size", 1000000);
  ctx.set(":pp-max-depth", 1000000);

  switch (argc) {
  case 4: {

    const char *theory = argv[1];
    const char *smt_filename = argv[2];
#if _DEBUG_INPUT_FORMULA_PARSER
    std::cerr << "Process " << smt_filename << std::endl;
#endif
    const char *smt_engine_code = argv[3];

    InputFormulaParser input(theory, smt_filename, smt_engine_code, ctx);

    AXDSignature sig(ctx, theory, input.getDecls());

    try {
      do {
	AXDInterpolant axd(sig,
			   // Input formulas
			   input.partA(), input.partB(),
			   // smt2 file name
			   smt_filename);

	switch (*smt_engine_code) {
	case '0':
	  axd.z3OutputFile();
	  break;
	case '1':
	  axd.mathsatOutputFile();
	  break;
	case '2':
	  axd.smtInterpolOutputFile();
	  break;
	default:
	  m_out << "Not valid SMT solver option." << std::endl;
	  // return 3;
	}
	m_out << axd << std::endl;
      } while (input.next());

    } catch (char const *e) {
      m_out << e << std::endl;
      // return 4;
    }

    return 0;
  }

  default:
    m_out << "Invalid number of arguments." << std::endl;
    return 5;
  }

  return 0;
}
