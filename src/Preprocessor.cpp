#include "Preprocess.h"
#include <exception>
#include <pthread.h>
#include <tuple>
#include <unordered_set>

Preprocessor::Preprocessor(char const * file):
  AXDSignature(),
  really_a_parser(ctx), 
  fresh_index(0), num_args_aux(0),
  part_a_array_var_ids({}), part_b_array_var_ids({}), common_array_var_ids({}),
  assertions((really_a_parser.from_file(file), 
        really_a_parser.assertions())),
  initial_index_vars(ctx)
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
  
  //std::cout << "Arrays A-local" << std::endl;
  //for(auto const & x : part_a_array_var_ids)
    //std::cout << x << std::endl;
  //std::cout << "Arrays B-local" << std::endl;
  //for(auto const & x : part_b_array_var_ids)
    //std::cout << x << std::endl;
  
  for(auto iterator_a = part_a_array_var_ids.begin(); 
      iterator_a != part_a_array_var_ids.end(); ++iterator_a){
    if(inSet(*iterator_a, part_b_array_var_ids))
      common_array_var_ids.insert(*iterator_a);
  }

  removeDuplicates(initial_index_vars);
  //std::cout << initial_index_vars << std::endl;
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
      if(formula.arg(0).num_args() > 0)
        flattenTerm(formula.arg(0), side);
      else{
        if(formula.arg(0).decl().range().name().str() == "Int")
          initial_index_vars.push_back(formula.arg(0));
        if(formula.arg(0).decl().range().name().str() == "ArraySort")
          updateArrayVarIds(formula.arg(0), side);
      }
      if(formula.arg(1).num_args() > 0)
        flattenTerm(formula.arg(1), side);
      else{
        if(formula.arg(1).decl().range().name().str() == "Int")
          initial_index_vars.push_back(formula.arg(1));
        if(formula.arg(1).decl().range().name().str() == "ArraySort")
          updateArrayVarIds(formula.arg(1), side);
      }
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
  if(f_name == "wr"){
    //std::cout << "-------A write function" << std::endl;
    if(term.arg(0).num_args() > 0)
      cojoin(term.arg(0), fresh_array_constant(), side);
    else
      updateArrayVarIds(term.arg(0), side);

    if(term.arg(1).num_args() > 0)
      cojoin(term.arg(1), fresh_index_constant(), side);
    else
      initial_index_vars.push_back(term.arg(1)); 

    if(term.arg(2).num_args() > 0)
      cojoin(term.arg(2), fresh_element_constant(), side);
  }
  if(f_name == "rd"){
    //std::cout << "-------A read function" << std::endl;
    if(term.arg(0).num_args() > 0)
      cojoin(term.arg(0), fresh_array_constant(), side);
    else
      updateArrayVarIds(term.arg(0), side);

    if(term.arg(1).num_args() > 0)
      cojoin(term.arg(1), fresh_index_constant(), side);
    else
      initial_index_vars.push_back(term.arg(1)); 
  }
  if(f_name == "diff"){
    //std::cout << "-------A diff function" << std::endl;
    if(term.arg(0).num_args() > 0)
      cojoin(term.arg(0), fresh_array_constant(), side);
    else
      updateArrayVarIds(term.arg(0), side);

    if(term.arg(1).num_args() > 0)
      cojoin(term.arg(1), fresh_array_constant(), side);
    else
      updateArrayVarIds(term.arg(1), side);
  }
}

z3::expr Preprocessor::fresh_array_constant(){
  return ctx.constant(("fresh_array_" + std::to_string(fresh_index++)).c_str(), array_sort);
}

z3::expr Preprocessor::fresh_element_constant(){
  return ctx.constant(("fresh_element_" + std::to_string(fresh_index++)).c_str(), element_sort);
}

z3::expr Preprocessor::fresh_index_constant(){
  return ctx.constant(("fresh_index_" + std::to_string(fresh_index++)).c_str(), int_sort);
}


void Preprocessor::cojoin(z3::expr const & e, z3::expr const & _new_const, SideInterpolant side){
  unsigned index_side = side != PART_A;

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

z3::expr_vector Preprocessor::getIndexVars() const {
  return initial_index_vars;
}

std::unordered_set<unsigned> Preprocessor::getALocalArrayVarIds() const {
  return part_a_array_var_ids;
}

std::unordered_set<unsigned> Preprocessor::getBLocalArrayVarIds() const {
  return part_b_array_var_ids;
}

std::unordered_set<unsigned> Preprocessor::getCommonArrayVarIds() const {
  return common_array_var_ids;
}

void Preprocessor::removeDuplicates(z3::expr_vector & terms){
  z3::expr_vector aux(ctx);
  std::unordered_set<unsigned> ids({});
  for(auto const & term : terms)
    if(!inSet(term.id(), ids)){
      ids.insert(term.id());
      aux.push_back(term);
    }
  terms = aux;
}
