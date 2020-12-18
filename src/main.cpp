#include "AXDInterpolant.h"
#include <cstdlib>

int main(int argc, char * argv[]){

  z3::set_param("proof", true);
  z3::context ctx;
  ctx.set(":pp-min-alias-size", 1000000);
  ctx.set(":pp-max-depth",      1000000);

  switch(argc){
    case 5:
      try {
        AXDInterpolant axd(ctx, 
            // Smt2 file name
            argv[2], 
            // Theory
            argv[1], 
            // Number of allowed attemps
            strtoul(argv[4], NULL, 0));
        switch(*argv[3]){
          case '0':
            axd.z3OutputFile();
            break;
          case '1':
            axd.mathsatOutputFile();
            break;
          case '2':
            axd.directComputation();
            break;
          default:
            std::cerr 
              << "Not valid SMT solver option."
              << std::endl;
            return 1;
        }
        std::cout << axd << std::endl;
        return 0;
      }
      catch(char const * e){
        std::cerr << e << std::endl;
        return 1;
      }
    default:
      std::cerr 
        << "Not allowed option." 
        << std::endl;
      return 1;
  }
}
