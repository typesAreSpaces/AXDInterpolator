#include "UAutomizerFileReader.h"
#include <sys/stat.h>

inline bool exists_file (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

int main(int argc, char * argv[]){

  if(argc != 6){
    std::cerr << "Invalid number of inputs.\n";
    return 0;
  }

  // argv[1] -> file path of smt file
  // argv[2] -> Solver Code
  // argv[3] -> file path of output file
  // argv[4] -> '0' -> test implementation
  //            '1' -> test other interpolation engines

  char const * file = argv[1];
  if(!exists_file(file)){
    std::cout << argv[1] << std::endl;
    std::cerr << "File doesnt exists.\n";
    return 0;
  }

  SMT_SOLVER curr_solver;
  switch(*argv[2]){
    case '0':
      curr_solver = Z3;
      break;
    case '1':
      curr_solver = MATHSAT;
      break;
    case '2':
      curr_solver = SMTINTERPOL;
      break;
    default:
      curr_solver = MATHSAT;
      break;
  }

  UAutomizerFileReader reader(
      curr_solver, 500, argv[3], *argv[4] == '0', *argv[5] == '0');
#if SINGLE_FORMULA
  reader.processSingleFile(file);
#else
  reader.process(file);
#endif

  return 0;
}
