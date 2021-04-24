#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <z3++.h>

enum BENCHMARK_EXIT_CODE { SUCCESS, FAILED, TIMEOUT };
enum SMT_SOLVER { Z3, MATHSAT, SMTINTERPOL };

class UAutomizerFileReader {

  std::string line, current_frame, current_file;
  int nesting_level, max_nesting_level;
  std::vector<std::string> stack_of_frames;
  SMT_SOLVER curr_solver;
  unsigned num_samples;

  bool isPushCmd() const;
  bool isPopCmd() const;
  bool isEchoCmd() const; 

  void action() const; 
  void reset(); 

  public:
  UAutomizerFileReader(SMT_SOLVER, unsigned);
    
  void process(char const *);
};
