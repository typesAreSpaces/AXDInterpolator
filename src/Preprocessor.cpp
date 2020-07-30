#include "Preprocess.h"

Preprocessor::Preprocessor(char const * file):
  AXDSignature(),
  really_a_parser(ctx), 
  fresh_index(0), num_args_aux(0),
  part_a_array_var_ids({}), part_b_array_var_ids({}),
  assertions((really_a_parser.from_file(file), 
        really_a_parser.assertions()))
{

  assert(assertions.size() == 2);

  // Processing Part-A
  num_args_aux = assertions[0].num_args();
  for(unsigned i = 0; i < num_args_aux; i++)
    flattenPredicate(assertions[0].arg(i), PART_A);

  // Processing Part-B
  num_args_aux = assertions[1].num_args();
  for(unsigned i = 0; i < num_args_aux; i++)
  ////for(unsigned i = 0; i <  assertions[1].num_args(); i++){
    ////std::cout << "@#$#!$!@#$ " << i << std::endl;
    flattenPredicate(assertions[1].arg(i), PART_B);
  //}
}

void Preprocessor::flattenPredicate(z3::expr const & formula, 
    SideInterpolant side){
  switch(formula.decl().decl_kind()){
    case Z3_OP_EQ: // ==
    case Z3_OP_DISTINCT: // !=
    case Z3_OP_GE: // >=
    case Z3_OP_LE: // <=
    case Z3_OP_GT: // >
    case Z3_OP_LT: // <
      //std::cout << formula.arg(0) << std::endl;
      //std::cout << formula.arg(1) << std::endl;
      if(formula.arg(0).num_args() > 0)
        flattenTerm(formula.arg(0), side);
      if(formula.arg(1).num_args() > 0)
        flattenTerm(formula.arg(1), side);
      break;
    default:
      throw "Error at "
        "Preprocessor::flattenPredicate(z3::expr const &)." 
        "Formula not in AXD";
  }
}

void Preprocessor::flattenTerm(z3::expr const & term, 
    SideInterpolant side){

  auto f_name = term.decl().name().str();
  if(f_name == "wr" || f_name == "rd"){
    //std::cout << "-------A " << f_name << " function" << std::endl;
    if(term.arg(0).num_args() > 0)
      cojoin(term.arg(0), side);
    else
      updateArrayVarIds(term.arg(0), side);
    
  }
  if(f_name == "diff"){
    //std::cout << "-------A diff function" << std::endl;
    if(term.arg(0).num_args() > 0)
      cojoin(term.arg(0), side);
    else
      updateArrayVarIds(term.arg(0), side);
    if(term.arg(1).num_args() > 0)
      cojoin(term.arg(1), side);
    else
      updateArrayVarIds(term.arg(1), side);
  }
}

z3::expr Preprocessor::fresh_array_constant(){
  return ctx.constant(("fresh_array_" + std::to_string(fresh_index++)).c_str(), array_sort);
}

void Preprocessor::cojoin(z3::expr const & e, SideInterpolant side){
  unsigned index_side = side != PART_A;

  z3::expr _new_const = fresh_array_constant();

  z3::expr_vector from(ctx);
  z3::expr_vector to(ctx);
  from.push_back(e);
  to.push_back(_new_const);

  z3::expr substitute = assertions[index_side].substitute(from, to); 
  z3::expr_vector args(ctx);
  for(unsigned i = 0; i < substitute.num_args(); i++)
    args.push_back(substitute.arg(i));
  args.push_back(_new_const == e);
  z3::expr _new = z3::mk_and(args);
  assertions.set(index_side, _new);
  num_args_aux++;
}

void Preprocessor::updateArrayVarIds(z3::expr const & e, 
    SideInterpolant side){
  switch(side){
    case PART_A:
      part_a_array_var_ids.insert(e.id());
      break;
    case PART_B:
      part_b_array_var_ids.insert(e.id());
      break;
  }
}

bool Preprocessor::isArrayVarCommon(z3::expr const & e){
  return inSet(e.id(), part_a_array_var_ids) 
    && inSet(e.id(), part_b_array_var_ids);
}

z3::expr Preprocessor::getPartA() const {
  return assertions[0];
}

z3::expr Preprocessor::getPartB() const {
  return assertions[1];
}
