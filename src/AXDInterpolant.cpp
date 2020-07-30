#include "AXDInterpolant.h"

AXDInterpolant::AXDInterpolant(char const * file) : 
  Preprocessor(file),
  solver(ctx),
  part_a(getPartA()), part_b(getPartB())
  
  //part_a({z3::expr_vector(ctx), z3::expr_vector(ctx)}), 
  //part_b({z3::expr_vector(ctx), z3::expr_vector(ctx)}),
  //fresh_index(0),
  //common_array_vars(ctx), 
  //common_array_var_names({}), aux_common_array_var_names({})
{
  //initial_split(a, part_a);
  //initial_split(b, part_b);

  //traverseA(a);
  //traverseB(b);
  //aux_common_array_var_names.clear();

//#if _DEBUG_AXD_INTER_
  //for(auto const & common_name : common_array_var_names)
    //std::cout << common_name << std::endl;
//#endif

  //for(auto const & form : part_a.second)
    //solver.add(form);
  //for(auto const & form : part_b.second)
    //solver.add(form);
  ////loop();
}

void AXDInterpolant::loop(){
  //unsigned stop;
  //while(solver.check() != z3::unsat){
    //std::cout << solver.check() << std::endl;
    //std::cin >> stop;
    //// TODO: implement the rest
  //}
}
