#include "Preprocess.h"

Preprocessor::Preprocessor(z3::context & ctx, char const * file):
  AXDSignature(ctx),
  really_a_parser(ctx), 
  fresh_index(0), current_conjs_in_input(0),
  input_part_a(ctx.bool_val(true)), 
  input_part_b(ctx.bool_val(true)),
  part_a_index_vars(ctx), part_b_index_vars(ctx),
  part_a_array_vars({}), part_b_array_vars({}), 
  common_array_vars({}){

  really_a_parser.from_file(file);
  z3::expr_vector assertions = really_a_parser.assertions();
  assert(assertions.size() == 2);
  input_part_a = assertions[0];
  input_part_b = assertions[1];

  // Processing Part-A
  current_conjs_in_input = input_part_a.num_args();
  for(unsigned i = 0; i < current_conjs_in_input; i++)
    flattenPredicate(input_part_a.arg(i), PART_A);

  // Processing Part-B
  current_conjs_in_input = input_part_b.num_args();
  for(unsigned i = 0; i < current_conjs_in_input; i++)
    flattenPredicate(input_part_b.arg(i), PART_B);

  // Set current_conjs_in_input to zero
  // because the variable is no longer needed
  current_conjs_in_input = 0;
  
#if _DEBUG_PREPROCESS_
  m_out << "Arrays A-local" << std::endl;
  m_out << part_a_array_vars << std::endl;
  m_out << "Arrays B-local" << std::endl;
  m_out << part_b_array_vars << std::endl;
#endif
   
  // Compute Common Array Var Ids
  for(auto iterator_a = part_a_array_vars.begin(); 
      iterator_a != part_a_array_vars.end(); ++iterator_a){
    if(inSet(*iterator_a, part_b_array_vars))
      common_array_vars.insert(*iterator_a);
  }

  removeDuplicates(part_a_index_vars);
  removeDuplicates(part_b_index_vars);

#if _DEBUG_PREPROCESS_
  m_out << part_a_index_vars << std::endl;
  m_out << part_b_index_vars << std::endl;
#endif
}

void Preprocessor::flattenPredicate(z3::expr const & formula, 
    SideInterpolant side){
  switch(formula.decl().decl_kind()){
    case Z3_OP_EQ:       // ==
    case Z3_OP_DISTINCT: // !=
    case Z3_OP_GE:       // >=
    case Z3_OP_LE:       // <=
    case Z3_OP_GT:       // >
    case Z3_OP_LT:       // <
      flattenTerm(formula.arg(0), side);
      flattenTerm(formula.arg(1), side);
      return;
    default:
      throw "Error at "
        "Preprocessor::flattenPredicate(z3::expr const &)." 
        "Formula not in AXD";
  }
}

void Preprocessor::flattenTerm(z3::expr const & term, 
    SideInterpolant side){
  if(term.num_args() > 0){
    auto f_name = func_name(term);
    if(f_name == "wr"){
      if(term.arg(0).num_args() > 0)
        cojoin(term.arg(0), fresh_array_constant(), side);
      else
        updateArrayVars(term.arg(0), side);

      if(term.arg(1).num_args() > 0)
        cojoin(term.arg(1), fresh_index_constant(), side);
      else
        updateIndexVars(term.arg(1), side);

      if(term.arg(2).num_args() > 0)
        cojoin(term.arg(2), fresh_element_constant(), side);
      return;
    }
    if(f_name == "rd"){
      if(term.arg(0).num_args() > 0)
        cojoin(term.arg(0), fresh_array_constant(), side);
      else
        updateArrayVars(term.arg(0), side);

      if(term.arg(1).num_args() > 0)
        cojoin(term.arg(1), fresh_index_constant(), side);
      else
        updateIndexVars(term.arg(1), side);
      return;
    }
    if(f_name == "diff"){
      if(term.arg(0).num_args() > 0)
        cojoin(term.arg(0), fresh_array_constant(), side);
      else
        updateArrayVars(term.arg(0), side);
      if(term.arg(1).num_args() > 0)
        cojoin(term.arg(1), fresh_array_constant(), side);
      else
        updateArrayVars(term.arg(1), side);
      return;
    }
    if(f_name == "pred"){
      if(term.arg(0).num_args() > 0)
        cojoin(term.arg(0), fresh_index_constant(), side);
      else
        updateIndexVars(term.arg(1), side);
      return;
    }
    // TODO: keep adding the rest of the signature
  }
  else{
    auto sort_name = sort_name(term);
    if(sort_name == "Int")
      updateIndexVars(term, side);
    if(sort_name == "ArraySort")
      updateArrayVars(term, side);
  }
}

void Preprocessor::cojoin(
    z3::expr const & e, 
    z3::expr const & _new_const, 
    SideInterpolant side){
  switch(side){
    case PART_A:
      cojoin_aux(input_part_a, e, _new_const);
      return;
    case PART_B:
      cojoin_aux(input_part_b, e, _new_const);
      return;
  }
}

void Preprocessor::cojoin_aux(
    z3::expr & predicate,
    z3::expr const & e, 
    z3::expr const & _new_const){
  z3::expr_vector from(ctx);
  z3::expr_vector to(ctx);
  from.push_back(e);
  to.push_back(_new_const);

  z3::expr substitute = predicate.substitute(from, to);
  z3::expr_vector args(ctx);
  for(unsigned i = 0; i < substitute.num_args(); i++)
    args.push_back(substitute.arg(i));
  args.push_back(_new_const == e);
  predicate = z3::mk_and(args);
  current_conjs_in_input++;
  return;
}

void Preprocessor::updateArrayVars(z3::expr const & e, 
    SideInterpolant side){
  switch(side){
    case PART_A:
      part_a_array_vars.insert(e);
      return;
    case PART_B:
      part_b_array_vars.insert(e);
      return;
  }
}

void Preprocessor::updateIndexVars(z3::expr const & e, 
    SideInterpolant side){
  switch(side){
    case PART_A:
      part_a_index_vars.push_back(e);
      return;
    case PART_B:
      part_b_index_vars.push_back(e);
      return;
  }
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

z3::expr Preprocessor::fresh_array_constant(){
  return ctx.constant((FRESH_ARRAY_PREFIX 
        + std::to_string(fresh_index++)).c_str(), array_sort);
}

z3::expr Preprocessor::fresh_element_constant(){
  return ctx.constant((FRESH_ELEMENT_PREFIX 
        + std::to_string(fresh_index++)).c_str(), element_sort);
}

z3::expr Preprocessor::fresh_index_constant(){
  return ctx.constant((FRESH_INDEX_PREFIX 
        + std::to_string(fresh_index++)).c_str(), int_sort);
}
