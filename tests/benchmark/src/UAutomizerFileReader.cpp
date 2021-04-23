#include "UAutomizerFileReader.h"
#include <z3++.h>

UAutomizerFileReader::UAutomizerFileReader(SMT_SOLVER smt_solver) : 
  line(""),
  current_frame(""),
  current_file(""),
  nesting_level(0),
  max_nesting_level(0),
  stack_of_frames({}),
  curr_solver(smt_solver)
{
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

void UAutomizerFileReader::action() const {
  std::string temp_file = "temp_" + current_file;
  std::string file_for_implementation = "axdinterpolator_" + current_file;

  // This std::ostream writes the top frame of formulas
  // in the stack_of_frames in order to check for satisfiability
  std::ofstream smt_file            (temp_file.c_str());
  // If formulas in the above frame are unsatisfiable, then
  // we generate a smtlib2 file for axd_interpolator
  // to consume
  std::ofstream axdinterpolator_file(file_for_implementation.c_str());

  for(auto const & x : stack_of_frames)
    smt_file << x << std::endl;
  smt_file << current_frame << std::endl;
  smt_file.close();

  z3::context ctx;
  z3::solver input_parser(ctx);
  input_parser.from_file(temp_file.c_str());

  if(input_parser.check() == z3::unsat){
    z3::expr_vector curr_assertions = input_parser.assertions();
    z3::expr_vector part_a(ctx), part_b(ctx);

    // ---------------------------------------------------------------------
    auto const & to_cnf_tactic = 
      z3::tactic(ctx, "tseitin-cnf");

    z3::goal goal_assertions(ctx);
    goal_assertions.add(z3::mk_and(curr_assertions));
    auto const & cnf_assertions = to_cnf_tactic(goal_assertions);

    assert(cnf_assertions.size() == 1);
    z3::expr const & curr_conjunction = cnf_assertions[0].as_expr();

    if(curr_conjunction.decl().decl_kind() != Z3_OP_AND)
      return;

    unsigned half_cnf = curr_conjunction.num_args()/2;
    for(unsigned i = 0; i < half_cnf; i++)
      part_a.push_back(curr_conjunction.arg(i));
    for(unsigned i = half_cnf; i < curr_conjunction.num_args(); i++)
      part_b.push_back(curr_conjunction.arg(i));

    if(part_a.size() == 0 || part_b.size() == 0)
      return;

    part_a.push_back(ctx.bool_val(true));
    part_b.push_back(ctx.bool_val(true));
    unsigned half_size = curr_assertions.size()/2;
    for(unsigned i = 0; i < half_size; i++)
      part_a.push_back(curr_assertions[i]);
    for(unsigned i = half_size; i < curr_assertions.size(); i++)
      part_b.push_back(curr_assertions[i]);
    // ---------------------------------------------------------------------

    axdinterpolator_file 
      << input_parser.to_smt2_decls_only();
    axdinterpolator_file 
      << "(assert "
      << z3::mk_and(part_a)
      << ")\n";
    axdinterpolator_file 
      << "(assert "
      << z3::mk_and(part_b)
      << ")\n";
    axdinterpolator_file 
      << "(check-sat)\n";
    axdinterpolator_file.close();

    char exec_command[1000];
    sprintf(exec_command, 
    "./../../bin/axd_interpolator QF_TO %s %u 1000;", 
    file_for_implementation.c_str(), curr_solver);
      int ret = system(exec_command);
    if(ret == 134){
      char log_command[1000];
      sprintf(log_command, 
          "echo \"%s\" \"%u\" %u >> /home/jose/benchmark_results.txt", 
          file_for_implementation.c_str(), curr_solver, TIMEOUT);
      system(log_command);
      system(("rm -rf " + temp_file).c_str());
      system(("rm -rf " + file_for_implementation).c_str());
    }
  }

  //int ret = system("[ -f /home/jose/Documents/GithubProjects/AXDInterpolator/ok.txt ]");
  //if(WEXITSTATUS(ret) != 0){
  //std::cout << "You will stop. Please input an integer to continue..." << std::endl;
  //int stop;
  //std::cin >> stop;
  //system(("mv temp_" + current_file + " ouch1.smt2").c_str());
  //system(("mv axdinterpolator_" + current_file + " ouch2.smt2").c_str());
  //}

  system(("rm -rf " + temp_file).c_str());
  system(("rm -rf " + file_for_implementation).c_str());
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
      action();
      current_frame = stack_of_frames.back();
      stack_of_frames.pop_back();
    }
    else if(isEchoCmd())
      continue;
    else
      current_frame += "\n" + line;

    if(nesting_level < 0)
      throw 
        "Error @ "
        "UAutomizerFileReader::process "
        "The number of pop operations are bigger than "
        "the number of push operations.";

    if(max_nesting_level < nesting_level)
      max_nesting_level = nesting_level;
  }

  smt_file.close();
  reset();
}
