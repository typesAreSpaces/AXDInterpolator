#include "AXDInterpolant.h"


int main(int argc, char * argv[]){

  z3::set_param("proof", true);
  z3::context ctx;

  char const * file_name = "./tests/smt2-files/paper_example.smt2";
  unsigned allowed_attempts = 10;

  switch(argc){
    case 1:
      try {
      AXDInterpolant(ctx, file_name, allowed_attempts);
      }
      catch(char const * e){
        std::cout << e << std::endl;
      }
      return 0;
    case 2:
      try {
      AXDInterpolant(ctx, argv[1], allowed_attempts);
      }
      catch(char const * e){
        std::cout << e << std::endl;
      }
      return 0;
    default:
      return 1;
  }
}
