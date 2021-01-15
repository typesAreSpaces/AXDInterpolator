#include "AXDInterpolant.h"
#include <cstdlib>

int main(int argc, char * argv[]){

  z3::set_param("proof", true);
  z3::context ctx;
  ctx.set(":pp-min-alias-size", 1000000);
  ctx.set(":pp-max-depth",      1000000);
  z3::solver input_parser(ctx);
  input_parser.from_file(argv[2]);

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
  assert(input_formula.size() == 2);
  z3::goal goal_a(ctx);
  z3::goal goal_b(ctx);
  goal_a.add(input_formula[0]);
  goal_b.add(input_formula[1]);
  auto const & part_a = my_tactic(goal_a);
  auto const & part_b = my_tactic(goal_b);

  switch(argc){
    case 5:
      try {
        for(unsigned i = 0; i < part_a.size(); i++){
          for(unsigned j = 0; j < part_b.size(); j++){
            AXDInterpolant axd(ctx, 
                // Input formula
                part_a[i].as_expr() 
                && part_b[j].as_expr(),
                // Smt2 file name
                argv[2], 
                // Theory
                argv[1], 
                // Number of allowed attemps
                strtoul(argv[4], NULL, 0));
            switch(*argv[3]){
              case '0':
                std::cout << axd.z3OutputFile() << std::endl;
                break;
              case '1':
                std::cout << axd.mathsatOutputFile() << std::endl;
                break;
              default:
                std::cerr 
                  << "Not valid SMT solver option."
                  << std::endl;
                return 1;
            }
          }
        }
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
