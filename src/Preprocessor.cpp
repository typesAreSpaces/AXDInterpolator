#include "Preprocess.h"

Preprocessor::Preprocessor(z3::context & ctx, char const * file):
  AXDSignature(ctx),
  really_a_parser(ctx), 
  fresh_index(0), num_args_aux(0),
  assertions((really_a_parser.from_file(file), 
        really_a_parser.assertions())),
  all_index_vars(ctx),
  part_a_array_var_ids({}), part_b_array_var_ids({}), common_array_var_ids({})
{
  assert(assertions.size() == 2);

  // Processing Part-A
  num_args_aux = assertions[0].num_args();
  for(unsigned i = 0; i < num_args_aux; i++)
    flattenPredicate(assertions[0].arg(i), PART_A);

  // Processing Part-B
  num_args_aux = assertions[1].num_args();
  for(unsigned i = 0; i < num_args_aux; i++)
    flattenPredicate(assertions[1].arg(i), PART_B);
  
#if _DEBUG_PREPROCESS_
  std::cout << "Arrays A-local" << std::endl;
  for(auto const & x : part_a_array_var_ids)
    std::cout << x << std::endl;
  std::cout << "Arrays B-local" << std::endl;
  for(auto const & x : part_b_array_var_ids)
    std::cout << x << std::endl;
#endif
   
  // Compute Common Array Var Ids
  for(auto iterator_a = part_a_array_var_ids.begin(); 
      iterator_a != part_a_array_var_ids.end(); ++iterator_a){
    if(inSet(*iterator_a, part_b_array_var_ids))
      common_array_var_ids.insert(*iterator_a);
  }

  removeDuplicates(all_index_vars);
#if _DEBUG_PREPROCESS_
  std::cout << all_index_vars << std::endl;
#endif
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
      flattenPredicateAux(formula.arg(0), side);
      flattenPredicateAux(formula.arg(1), side);
      return;
    default:
      throw "Error at "
        "Preprocessor::flattenPredicate(z3::expr const &)." 
        "Formula not in AXD";
  }
}

void Preprocessor::flattenPredicateAux(z3::expr const & term, 
    SideInterpolant side){
  if(term.num_args() > 0)
    flattenTerm(term, side);
  else{
    auto sort_name = term.decl().range().name().str(); 
    if(sort_name == "Int")
      all_index_vars.push_back(term);
    if(sort_name == "ArraySort")
      updateArrayVarIds(term, side);
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
      all_index_vars.push_back(term.arg(1)); 

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
      all_index_vars.push_back(term.arg(1)); 
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
      part_a_array_var_ids.insert(e);
      return;
    case PART_B:
      part_b_array_var_ids.insert(e);
      return;
  }
}

bool Preprocessor::isArrayVarCommon(z3::expr const & e){
  return inSet(e, part_a_array_var_ids) 
    && inSet(e, part_b_array_var_ids);
}

void Preprocessor::removeDuplicates(z3::expr_vector & terms){
  z3::expr_vector aux(ctx);
  z3_expr_set ids({});
  for(auto const & term : terms)
    if(!inSet(term, ids)){
      ids.insert(term);
      aux.push_back(term);
    }
  terms = aux;
}
