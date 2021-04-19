#include "AXDInterpolant.h"
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

inline bool exists_file (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

int main(int argc, char * argv[]){

  z3::context ctx;
  //z3::set_param("proof", true);
  ctx.set(":pp-min-alias-size", 1000000);
  ctx.set(":pp-max-depth",      1000000);
  system("rm -rf ok.txt");

  switch(argc){
    case 5:
      {
        z3::solver input_parser(ctx);
        if(!exists_file(argv[2])){
          std::cout
            << "File not found." 
            << std::endl;
          return 0;
        }

        input_parser.from_file(argv[2]);
        std::string decls = input_parser.to_smt2_decls_only();
        AXDSignature sig(ctx, argv[1], decls);

        auto const & to_cnf_tactic = 
          z3::tactic(ctx, "tseitin-cnf");
        auto const & to_dnf_tactic = 
          z3::repeat(
              z3::tactic(ctx, "split-clause") 
              | z3::tactic(ctx, "skip"));
        auto const & my_tactic = 
          to_cnf_tactic & to_dnf_tactic;

        auto const & input_formula
          = input_parser.assertions();
        if(input_formula.size() == 0){
          std::cout 
            << "No assertions in input file" 
            << std::endl;
          return 0;
        }
        assert(input_formula.size() == 2);
        z3::goal goal_a(ctx);
        z3::goal goal_b(ctx);
        goal_a.add(input_formula[0]);
        goal_b.add(input_formula[1]);
        auto const & part_a = my_tactic(goal_a);
        auto const & part_b = my_tactic(goal_b);

        try {
          for(unsigned i = 0; i < part_a.size(); i++){
            for(unsigned j = 0; j < part_b.size(); j++){
              AXDInterpolant axd(
                  sig,
                  // Input formulas
                  part_a[i].as_expr(),
                  part_b[j].as_expr(),
                  // Smt2 file name
                  argv[2], 
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
                  axd.smtInterpolOutputFile();
                  break;
                default:
                  std::cout
                    << "Not valid SMT solver option."
                    << std::endl;
                  return 0;
              }
              std::cout << axd << std::endl;
            }
          }
          system("touch ok.txt");
          return 0;
        }
        catch(char const * e){
          std::cout << e << std::endl;
          return 0;
        }
      }

    default:
      std::cout
        << "Invalid number of arguments." 
        << std::endl;
      return 0;
  }
}
