#include <iostream>
#include <cassert>
#include <regex>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <regex>
#include "z3++.h"

#define SINGLE_FORMULA 0
#define REPORT_BAD_CASES 0

#define TEMP_FILE_SETUP\
  std::string temp_file =\
  "temp_" + name_solver + "_" + current_file;\
  std::ofstream smt_file (temp_file.c_str());\
  \
  for(auto const & x : stack_of_frames)\
  smt_file << x << std::endl;\
  smt_file << current_frame << std::endl;\
  smt_file.close();\

#define BENCHMARK_COMMAND(WRITER, EXEC_COMMAND, LOG_COMMAND)\
  z3::context ctx;\
  z3::solver input_parser(ctx, "QF_AUFLIA");\
  input_parser.from_file(temp_file.c_str());\
  \
  for(auto const & x : input_parser.assertions()){\
    if(hasNonSupportedSymbols(x)){\
      system(("rm -rf " + temp_file).c_str());\
      return;\
    }\
  }\
  \
  if(input_parser.check() == z3::unsat){\
    std::string file_for_implementation =\
    "axdinterpolator_" +  name_solver + "_" + current_file;\
    std::ofstream axdinterpolator_file(file_for_implementation.c_str());\
    \
    z3::expr_vector curr_assertions = input_parser.assertions();\
    z3::expr_vector part_a(ctx), part_b(ctx);\
    \
    auto const & to_cnf_tactic = \
    z3::tactic(ctx, "tseitin-cnf");\
    \
    z3::goal goal_assertions(ctx);\
    goal_assertions.add(z3::mk_and(curr_assertions));\
    auto const & cnf_assertions = to_cnf_tactic(goal_assertions);\
    \
    assert(cnf_assertions.size() == 1);\
    z3::expr const & curr_conjunction = cnf_assertions[0].as_expr();\
    \
    unsigned total_size_cnf = curr_conjunction.num_args();\
    unsigned half_size_cnf = total_size_cnf/2;\
    if(curr_conjunction.decl().decl_kind() != Z3_OP_AND\
        || half_size_cnf == 0\
        || (total_size_cnf - half_size_cnf) == 0 \
        ){\
      axdinterpolator_file.close();\
      system(("rm -rf " + file_for_implementation).c_str());\
      system(("rm -rf " + temp_file).c_str());\
      return;\
    }\
    \
    part_a.push_back(ctx.bool_val(true));\
    part_b.push_back(ctx.bool_val(true));\
    for(unsigned i = 0; i < half_size_cnf; i++)\
    part_a.push_back(curr_conjunction.arg(i));\
    for(unsigned i = half_size_cnf; i < total_size_cnf; i++)\
    part_b.push_back(curr_conjunction.arg(i));\
    \
    WRITER;\
    \
    char exec_command[1000];\
    EXEC_COMMAND;\
    int ret = system(exec_command);\
    char log_command[1000];\
    LOG_COMMAND;\
    system(log_command);\
    \
    system(("rm -rf " + file_for_implementation).c_str());\
  }

enum BENCHMARK_EXIT_CODE { SUCCESS, FAILED, TIMEOUT };
enum SMT_SOLVER { Z3, MATHSAT, SMTINTERPOL };

class UAutomizerFileReader {

  std::string line, current_frame, current_file;
  int nesting_level, max_nesting_level;
  unsigned num;
  std::vector<std::string> stack_of_frames;
  SMT_SOLVER curr_solver;
  std::string const name_solver;
  unsigned num_samples;
  char * const file_statistics;
  char implementation;

  bool        hasNonSupportedSymbols(z3::expr const &) const;
  std::string nameAssertionsZ3(std::string const &) const;
  std::string nameAssertionsMathsat(std::string const &) const;

  bool isPushCmd() const;
  bool isPopCmd() const;
  bool isEchoCmd() const; 

  void testAXDInterpolator() const; 
  void testCAXDInterpolator() const; 
  void testOtherSolvers(); 
  void reset(); 

  public:
  UAutomizerFileReader(SMT_SOLVER, unsigned, char * const, char);
    
  void process(char const *);
  void processSingleFile(char const *);
};
