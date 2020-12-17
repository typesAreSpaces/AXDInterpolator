#include "AXDInterpolant.h"

#define _Z3_OUTPUT_FILE_            0
#define _MATHSAT5_OUTPUT_FILE_      1
#define _DIRECT_INTERP_COMPUTATION_ 0

int main(int argc, char * argv[]){

  z3::set_param("proof", true);
  z3::context ctx;
  ctx.set(":pp-min-alias-size", 1000000);
  ctx.set(":pp-max-depth",      1000000);

  // If the application is executed using 
  // no arguments, then a file_name pointing
  // a smt2 file should be specified
  char const * file_name = "./tests/smt2-files/paper_example.smt2";
  unsigned allowed_attempts = 10;

  switch(argc){
    case 1:
      try {
        AXDInterpolant axd(
            ctx, file_name, allowed_attempts);
#if _Z3_OUTPUT_FILE_
        axd.z3OutputFile();
        std::cout << axd << std::endl;
#endif
#if _MATHSAT5_OUTPUT_FILE_
        axd.mathsatOutputFile();
        std::cout << axd << std::endl;
#endif
#if _DIRECT_INTERP_COMPUTATION_
        axd.directComputation();
        std::cout << axd << std::endl;
#endif
        return 0;
      }
      catch(char const * e){
        std::cerr << e << std::endl;
        return 1;
      }
    case 2:
      try {
        AXDInterpolant axd(
            ctx, argv[1], allowed_attempts);
#if _Z3_OUTPUT_FILE_
        axd.z3OutputFile();
        std::cout << axd << std::endl;
#endif
#if _MATHSAT5_OUTPUT_FILE_
        axd.mathsatOutputFile();
        std::cout << axd << std::endl;
#endif
#if _DIRECT_INTERP_COMPUTATION_
        axd.directComputation();
        std::cout << axd << std::endl;
#endif
        return 0;
      }
      catch(char const * e){
        std::cerr << e << std::endl;
        return 1;
      }
    default:
      std::cerr << "Not allowed option." << std::endl;
      return 1;
  }
}
