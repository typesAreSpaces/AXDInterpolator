#include "StandardInput.h"

StandardInput::DiffTableEntry::DiffTableEntry(){
}

StandardInput::DiffTable::DiffTable(){
}

StandardInput::WriteVector::WriteVector() : 
  m_vector()
{
}

void StandardInput::WriteVector::add(z3::expr const & a, z3::expr const & b, z3::expr const & i){
  m_vector.push_back(std::make_tuple(a, b, i));
}


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
            || current_arg.arg(1).decl().name().str() == "diff" ){
          part_1.push_back(orientBinPredicate(current_arg));
        }
        else
          part_2.push_back(orientBinPredicate(current_arg));
        break;
      case Z3_OP_DISTINCT: // !=
      case Z3_OP_GE: // >=
      case Z3_OP_LE: // <=
      case Z3_OP_GT: // >
      case Z3_OP_LT: // <
        part_2.push_back(orientBinPredicate(current_arg));
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
  for(auto const & equation : part_1){
    auto f_name = equation.arg(1).decl().name().str();
    if(f_name == "wr"){
      write_vector.add(
          equation.arg(0), 
          equation.arg(1).arg(0), 
          equation.arg(1).arg(1));
    }
    else if(f_name == "diff"){
    }
  }
}

void StandardInput::update(){
}

z3::expr_vector const & StandardInput::getPart_1() const {
  return part_1;
}

z3::expr_vector const & StandardInput::getPart_2() const {
  return part_2;
}

z3::expr StandardInput::orientBinPredicate(z3::expr const & eq){
  if(eq.arg(0).num_args() > eq.arg(1).num_args())
    return eq.arg(1) == eq.arg(0);
  return eq;
}
