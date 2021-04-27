#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <regex>
#include <z3++.h>

enum BENCHMARK_EXIT_CODE { SUCCESS, FAILED, TIMEOUT };
enum SMT_SOLVER { Z3, MATHSAT, SMTINTERPOL };

class UAutomizerFileReader {

  std::string line, current_frame, current_file;
  int nesting_level, max_nesting_level;
  std::vector<std::string> stack_of_frames;
  SMT_SOLVER curr_solver;
  std::string const name_solver;
  unsigned num_samples;
  char * const file_statistics;

  bool hasQuantifier(z3::expr const &) const;
  std::string fromImplToNamed(std::string const &) const;
  std::string fromImplToNamedMathsat(std::string const &) const;

  bool isPushCmd() const;
  bool isPopCmd() const;
  bool isEchoCmd() const; 

  void testAXDInterpolator() const; 
  void testOtherSolvers() const; 
  void reset(); 

  public:
  UAutomizerFileReader(SMT_SOLVER, unsigned, char * const);
    
  void process(char const *);
};
