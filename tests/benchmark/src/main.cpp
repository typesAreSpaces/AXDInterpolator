#include "UAutomizerFileReader.h"
#include <sys/stat.h>

inline bool exists_file (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

int main(int argc, char * argv[]){

  if(argc != 3){
    std::cerr << "Invalid number of inputs.\n";
    return 0;
  }

  char const * file = argv[1];
  if(!exists_file(file)){
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

  UAutomizerFileReader reader(curr_solver);
  reader.process(file);

  return 0;
}
