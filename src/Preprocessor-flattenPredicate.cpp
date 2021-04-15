#include "Preprocess.h"

void Preprocessor::flattenPredicate(
    z3::expr const & formula, 
    SideInterpolant side,
    unsigned & current_conjs_in_input){
  switch(formula.decl().decl_kind()){
    case Z3_OP_EQ:       // ==
      {
        auto const & lhs_form = lhs(formula);
        if(lhs_form.num_args() == 0){
          updateVarsDB(
              lhs_form, side);
          flattenTerm(
              rhs(formula), side, 
              current_conjs_in_input);
        }
        else
          flattenPredicateAux(
              formula, side,
              current_conjs_in_input);
      }
      return;
    case Z3_OP_DISTINCT: // !=
    case Z3_OP_GE:       // >=
    case Z3_OP_LE:       // <=
    case Z3_OP_GT:       // >
    case Z3_OP_LT:       // <
      flattenPredicateAux(
          formula, side,
          current_conjs_in_input);
      return;
    case Z3_OP_UNINTERPRETED:
      assert(formula.get_sort().is_bool());
      flattenTerm(
          formula, side,
          current_conjs_in_input);
      return;
    default:
      throw 
        "Error at "
        "Preprocessor::flattenPredicate(z3::expr const &). "
        "Formula not in AXD";
  }
}

// Takes an atomic predicate, except equality, 
// i.e. B(a, b) and updates the correspondant
// input side with fresh constants x, y if a 
// and b arent constants (respectively) obtaining
// input[side](x/a)(y/t) \land x = a \land y = b
void Preprocessor::flattenPredicateAux(
    z3::expr const & atomic_predicate, SideInterpolant side,
    unsigned & current_conjs_in_input){

  z3::expr_vector old_terms(sig.ctx); 
  z3::expr_vector fresh_consts(sig.ctx);
  z3::expr_vector temp_predicates(sig.ctx);

  auto const & lhs_atom = lhs(atomic_predicate);
  if(lhs_atom.num_args() > 0){
    old_terms.push_back(lhs_atom);
    fresh_consts.push_back(
        fresh_constant(_get_sort(lhs_atom)));
  }
  auto const & rhs_atom = rhs(atomic_predicate);
  if(rhs_atom.num_args() > 0){
    old_terms.push_back(rhs_atom);
    fresh_consts.push_back(
        fresh_constant(_get_sort(rhs_atom)));
  }

  switch(side){
    case PART_A:
      {
        for(unsigned i = 0; i < input_part_a.size(); i++)
          temp_predicates.push_back(
              input_part_a[i].substitute(old_terms, fresh_consts)
              );

        unsigned j = old_terms.size();
        while(j--)
          temp_predicates.push_back(fresh_consts[j] == old_terms[j]);

        input_part_a = temp_predicates;
        current_conjs_in_input += old_terms.size();
        return;
      }
    case PART_B:
      {
        for(unsigned i = 0; i < input_part_b.size(); i++){
          temp_predicates.push_back(
              input_part_b[i].substitute(old_terms, fresh_consts)
              );
        }

        unsigned j = old_terms.size();
        while(j--)
          temp_predicates.push_back(fresh_consts[j] == old_terms[j]);

        input_part_b = temp_predicates;
        current_conjs_in_input += old_terms.size();
      }
      return;
  }
}

void Preprocessor::flattenTerm(z3::expr const & term, 
    SideInterpolant side,
    unsigned & current_conjs_in_input){
  if(term.num_args() > 0){
    auto f_name = func_name(term);
    for(unsigned i = 0; i < term.num_args(); i++){
      auto const & curr_arg = term.arg(i);
      auto const & type_arg = _get_sort(curr_arg);
      if(curr_arg.num_args() > 0)
        cojoin(curr_arg, fresh_constant(type_arg), side,
            current_conjs_in_input);
      else
        updateVarsDB(curr_arg, side); 
    }
  }
  else
    updateVarsDB(term, side);
}

void Preprocessor::cojoin(
    z3::expr const & old_term, 
    z3::expr const & new_const, 
    SideInterpolant side,
    unsigned & current_conjs_in_input){
  switch(side){
    case PART_A:
      cojoin_aux(input_part_a, old_term, new_const,
          current_conjs_in_input);
      return;
    case PART_B:
      cojoin_aux(input_part_b, old_term, new_const,
          current_conjs_in_input);
      return;
  }
}

void Preprocessor::cojoin_aux(
    z3::expr_vector & predicates,
    z3::expr const & old_term, 
    z3::expr const & new_const,
    unsigned & current_conjs_in_input){

  z3::expr_vector from(sig.ctx), to(sig.ctx);
  from.push_back(old_term);
  to.push_back(new_const);

  z3::expr_vector temp_predicates(sig.ctx);
  for(unsigned i = 0; i < predicates.size(); i++)
    temp_predicates.push_back(
        predicates[i].substitute(from, to));
  temp_predicates.push_back(new_const == old_term);
  predicates = temp_predicates;
  current_conjs_in_input++;
  return;
}

void Preprocessor::updateArrayVars(
    z3::expr const & e, 
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

void Preprocessor::updateIndexVars(
    z3::expr const & e, 
    SideInterpolant side){
  switch(side){
    case PART_A:
      part_a_index_vars.push(e);
      return;
    case PART_B:
      part_b_index_vars.push(e);
      return;
  }
}

void Preprocessor::updateVarsDB(
    z3::expr const & e, 
    SideInterpolant side){
  if(sig.isArraySort(e.get_sort())){
    updateArrayVars(e, side);
    return;
  }
  if(e.is_int()){
    updateIndexVars(e, side);
    return;
  }
}

z3::expr Preprocessor::fresh_index_constant(){
  return sig.ctx.constant((FRESH_INDEX_PREFIX 
        + std::to_string(fresh_num++)).c_str(), sig.int_sort);
}

z3::expr Preprocessor::fresh_array_constant(z3::sort const & s){
  return sig.ctx.constant((FRESH_ARRAY_PREFIX 
        + std::to_string(fresh_num++)).c_str(), sig.getArraySortBySort(s));
}

z3::expr Preprocessor::fresh_constant(z3::sort const & s){
  if(sig.isArraySort(s))
    return fresh_array_constant(s);
  if(s.is_int())
    return fresh_index_constant();

  throw 
    "Problem @ Preprocessor::fresh_constant. "
    "No support for the given sort.";
}
