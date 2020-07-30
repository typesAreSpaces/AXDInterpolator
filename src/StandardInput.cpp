#include "StandardInput.h"

StandardInput::StandardInput(z3::expr const & e) : 
  part_1(e.ctx()), part_2(e.ctx())
{
  assert(e.decl().decl_kind() == Z3_OP_AND);
  for(unsigned i = 0; i < e.num_args(); i++){
    auto current_arg = e.arg(i);
    switch(current_arg.decl().decl_kind()){
      case Z3_OP_EQ: // ==
        if(current_arg.arg(0).decl().range().name().str() == "ArraySort" 
            || current_arg.arg(0).decl().name().str() == "diff" 
            || current_arg.arg(1).decl().name().str() == "diff" )
          part_1.push_back(current_arg);
        else
          part_2.push_back(current_arg);
        break;
      case Z3_OP_DISTINCT: // !=
      case Z3_OP_GE: // >=
      case Z3_OP_LE: // <=
      case Z3_OP_GT: // >
      case Z3_OP_LT: // <
        part_2.push_back(current_arg);
        break;
      default:
        throw "Invalid formula.";
    }
  }
#if _DEBUG_STDINPUT_
  std::cout << "Done" << std::endl;
  std::cout << "Part 1: " << part_1 << std::endl;
  std::cout << "Part 2: " << part_2 << std::endl;
#endif
}

void StandardInput::update(){
}
