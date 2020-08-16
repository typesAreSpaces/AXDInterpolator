#include "AXDInterpolant.h"


int main(int argc, char * argv[]){

  z3::set_param("proof", true);
  z3::context ctx;
  ctx.set(":pp-min-alias-size", 1000000);
  ctx.set(":pp-max-depth", 1000000);

  // If the application only is run using 
  // no arguments, then a file_name pointing
  // a smt2 file should be specified
  char const * file_name = "./tests/smt2-files/paper_example.smt2";
  unsigned allowed_attempts = 10;

  switch(argc){
    case 1:
      try {
        AXDInterpolant(ctx, file_name, allowed_attempts);
        return 0;
      }
      catch(char const * e){
        std::cout << e << std::endl;
        return 1;
      }
    case 2:
      try {
        AXDInterpolant(ctx, argv[1], allowed_attempts);
        return 0;
      }
      catch(char const * e){
        std::cout << e << std::endl;
        return 1;
      }
    default:
      return 1;
  }
}
