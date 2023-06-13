#include "UAutomizerFileReader.h"

UAutomizerFileReader::UAutomizerFileReader(
    SMT_SOLVER smt_solver, 
    unsigned num_samples, 
    char * const file, 
    char implementation) : 
  line(""),
  current_frame(""),
  current_file(""),
  nesting_level(0),
  max_nesting_level(0),
  num(0),
  stack_of_frames({}),
  curr_solver(smt_solver),
  name_solver(
      smt_solver == Z3 ? "Z3" 
      : smt_solver == MATHSAT ? "MATHSAT" 
      : "SMTINTERPOL"),
  num_samples(num_samples),
  file_statistics(file),
  implementation(implementation)
{
}

bool UAutomizerFileReader::hasNonSupportedSymbols(z3::expr const & e) const {
  if(e.is_quantifier())
    return true;
  if(e.is_var())
    return true;
  if(e.is_app()){
    if(e.is_array() && e.get_sort().array_domain().is_bv())
      return true;
    if(e.is_bv())
      return true;
    switch(e.decl().decl_kind()){
      case Z3_OP_ITE:
      case Z3_OP_REM:
      case Z3_OP_MOD:
      case Z3_OP_DIV:
      case Z3_OP_IDIV:
        return true;
      case Z3_OP_MUL:
        {
          unsigned num_numerals = 0, curr_num_args = e.num_args();
          for(unsigned i = 0; i < curr_num_args; i++)
            if(e.is_numeral())
              num_numerals++;

          if(curr_num_args - num_numerals >= 2)
            return true;
          return false;
        }
      default:
        for(unsigned i = 0; i < e.num_args(); i++)
          if (hasNonSupportedSymbols(e.arg(i)))
            return true;
        break;
    }
  }
  return false;
}

bool UAutomizerFileReader::isPushCmd() const {
  return line.find("push") != std::string::npos;
}

bool UAutomizerFileReader::isPopCmd() const {
  return line.find("pop") != std::string::npos;
}

bool UAutomizerFileReader::isEchoCmd() const {
  return line.find("echo") != std::string::npos;
}

void UAutomizerFileReader::reset(){
  line = "";
  current_frame = "";
  nesting_level = 0;
  max_nesting_level = 0;
  stack_of_frames.clear();
}

void UAutomizerFileReader::process(char const * file_path){
  std::string string_path = file_path;
  current_file = 
    string_path.substr(string_path.find_last_of("/\\") + 1);
  std::ifstream smt_file;
  smt_file.open(file_path);

  while(std::getline(smt_file, line)){

    if(isPushCmd()){
      nesting_level++;
      stack_of_frames.push_back(current_frame);
      current_frame = "";
    }
    else if(isPopCmd()){
      nesting_level--;
      if(num_samples == 0){
        smt_file.close();
        reset();
        return;
      }
      num_samples--;

      switch (implementation) {
      case '0':
	testCAXDInterpolator();
	break;
      case '1':
	testAXDInterpolator();
	break;
      case '2':
	testOtherSolvers();
	break;
      }
      
      current_frame = stack_of_frames.back();
      stack_of_frames.pop_back();
    } else if (isEchoCmd())
      continue;
    else
      current_frame += "\n" + line;

    if (nesting_level < 0)
      throw "Error @ "
	    "UAutomizerFileReader::process "
	    "The number of pop operations are bigger than "
	    "the number of push operations.";

    if (max_nesting_level < nesting_level)
      max_nesting_level = nesting_level;
  }

  smt_file.close();
  reset();
}

void UAutomizerFileReader::processSingleFile(char const *file_path) {

  std::string string_path = file_path;
  current_file = string_path.substr(string_path.find_last_of("/\\") + 1);

  std::cout << file_path << std::endl;

  z3::context ctx;
  z3::solver input_parser(ctx);

  input_parser.from_file(file_path);

  for (auto const &x : input_parser.assertions())
    if (hasNonSupportedSymbols(x))
      return;

  if (input_parser.check() == z3::unsat) {
    std::string file_for_implementation =
	"axdinterpolator_" + name_solver + "_" + current_file;
    std::ofstream axdinterpolator_file(file_for_implementation.c_str());

    z3::expr_vector curr_assertions = input_parser.assertions();
    z3::expr_vector part_a(ctx), part_b(ctx);

    auto const &to_cnf_tactic = z3::tactic(ctx, "tseitin-cnf");

    z3::goal goal_assertions(ctx);
    goal_assertions.add(z3::mk_and(curr_assertions));
    auto const &cnf_assertions = to_cnf_tactic(goal_assertions);

    assert(cnf_assertions.size() == 1);
    z3::expr const &curr_conjunction = cnf_assertions[0].as_expr();

    unsigned total_size_cnf = curr_conjunction.num_args();
    unsigned half_size_cnf = total_size_cnf / 2;
    if (curr_conjunction.decl().decl_kind() != Z3_OP_AND ||
	half_size_cnf == 0 || (total_size_cnf - half_size_cnf) == 0) {
      axdinterpolator_file.close();
      system(("rm -rf " + file_for_implementation).c_str());
      return;
    }

    part_a.push_back(ctx.bool_val(true));
    part_b.push_back(ctx.bool_val(true));
    for (unsigned i = 0; i < half_size_cnf; i++)
      part_a.push_back(curr_conjunction.arg(i));
    for (unsigned i = half_size_cnf; i < total_size_cnf; i++)
      part_b.push_back(curr_conjunction.arg(i));

    // WRITER;
    z3::solver tseitin_solver(ctx, "QF_AUFLIA");
    tseitin_solver.add(z3::mk_and(part_a));
    tseitin_solver.add(z3::mk_and(part_b));

    axdinterpolator_file << tseitin_solver.to_smt2();
    axdinterpolator_file.close();

    char exec_command[1000];
    // EXEC_COMMAND;
    sprintf(exec_command, "./../../bin/caxd_interpolator QF_LIA %s %u;",
	    file_for_implementation.c_str(), curr_solver);

    int ret = system(exec_command);
    char log_command[1000];
    // LOG_COMMAND;
#if REPORT_BAD_CASES
    if (ret != 0 && ret != 152) {
      char complain_command[1000];
      sprintf(complain_command,
	      "echo File: \"%s\" Solver Code: \"%u\" Sample Id: %d "
	      "Exit Code: %d >> /home/jose/bad_cases.txt",
	      file_for_implementation.c_str(), curr_solver, 500 - num_samples,
	      ret);
      system(complain_command);
      system(("mv " + file_for_implementation + " ~/" +
	      file_for_implementation + std::to_string(500 - num_samples))
		 .c_str());
    }
#endif
    sprintf(log_command,
	    "echo File: \"%s\" Solver Code: \"%u\" Exit Code: %d >> \"%s\"",
	    file_for_implementation.c_str(), curr_solver, ret, file_statistics);

    system(log_command);

    system(("rm -rf " + file_for_implementation).c_str());
  }

  reset();
}
