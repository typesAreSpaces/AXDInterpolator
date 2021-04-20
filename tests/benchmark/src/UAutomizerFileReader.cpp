#include "UAutomizerFileReader.h"
#include <z3++.h>

UAutomizerFileReader::UAutomizerFileReader() : 
  line(""),
  current_frame(""),
  current_file(""),
  nesting_level(0),
  max_nesting_level(0),
  stack({}) 
{
}

bool UAutomizerFileReader::isPushCmd(){
  return line.find("push") != std::string::npos;
}

bool UAutomizerFileReader::isPopCmd(){
  return line.find("pop") != std::string::npos;
}

bool UAutomizerFileReader::isEchoCmd(){
  return line.find("echo") != std::string::npos;
}

void UAutomizerFileReader::action(){
  std::string temp_file = "temp_" + current_file;
  std::string file_for_implementation = "axdinterpolator_" + current_file;
  // This std::ostream writes the top frame of formulas
  // in the stack in order to check for satisfiability
  std::ofstream smt_file            (temp_file.c_str());
  // If formulas in the above frame are unsatisfiable, then
  // we generate a smtlib2 file for axd_interpolator
  // to consume
  std::ofstream axdinterpolator_file(file_for_implementation.c_str());

  for(auto const & x : stack)
    smt_file << x << std::endl;
  smt_file << current_frame << std::endl;
  smt_file.close();

  z3::context ctx;
  z3::solver input_parser(ctx);
  input_parser.from_file(temp_file.c_str());

  if(input_parser.check() == z3::unsat){
    //TODO: 
    // Extract the assertions and make a non-trivial
    // splitting, i.e. there show be at least a common
    // symbol between the two formulas in the splitting

    z3::expr_vector curr_assertions = input_parser.assertions();
    std::cout << curr_assertions << std::endl;

    axdinterpolator_file 
      << input_parser.to_smt2_decls_only();
    axdinterpolator_file 
      << "(assert "
      << z3::mk_and(input_parser.assertions())
      << ")\n";
    axdinterpolator_file 
      << "(assert false)\n";
    axdinterpolator_file 
      << "(check-sat)\n";
    axdinterpolator_file.close();

    system((
          "pushd ./../../ > /dev/null;"
          "./bin/axd_interpolator QF_TO ./tests/benchmark/" 
          + file_for_implementation + " 1 1000;" 
          "popd > /dev/null;"
          ).c_str());
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
  stack.clear();
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
      stack.push_back(current_frame);
      current_frame = "";
    }
    else if(isPopCmd()){
      nesting_level--;
      action();
      current_frame = stack.back();
      stack.pop_back();
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
