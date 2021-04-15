#include "Preprocess.h"

#define SUBSTITUTE_AND_ADJOINT(INPUT_PART)\
  for(unsigned i = 0; i < INPUT_PART.size(); i++)\
  temp_predicates.push_back(\
      INPUT_PART[i].substitute(old_terms, fresh_consts)\
      );\
      \
      for(unsigned i = 0; i < old_terms.size(); i++)\
      temp_predicates.push_back(fresh_consts[i] == old_terms[i]);\
      \
      INPUT_PART = temp_predicates;\
      current_conjs_in_input += old_terms.size();

#define SUBSTITUTE_AND_ADJOINT_ONE(INPUT_PART)\
  for(unsigned i = 0; i < INPUT_PART.size(); i++)\
  temp_predicates.push_back(\
      INPUT_PART[i].substitute(from, to));\
      temp_predicates.push_back(new_const == old_term);\
      INPUT_PART = temp_predicates;\
      current_conjs_in_input++;

void Preprocessor::flattenPredicate(
    z3::expr const & formula, 
    SideInterpolant side,
    unsigned & current_conjs_in_input){
  switch(func_kind(formula)){
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

// Takes a binary atomic predicate,
// i.e. B(x, y) and updates the correspondant
// input side with fresh constants a, b if x
// and y arent constants (respectively) obtaining
// B(x, y)[x->a, y->b] \land x = a \land y = b
void Preprocessor::flattenPredicateAux(
    z3::expr const & atomic_predicate, 
    SideInterpolant side,
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
      SUBSTITUTE_AND_ADJOINT(input_part_a);
      return;
    case PART_B:
      SUBSTITUTE_AND_ADJOINT(input_part_b);
      return;
  }
}

void Preprocessor::flattenTerm(
    z3::expr const & term, 
    SideInterpolant side,
    unsigned & current_conjs_in_input){
  if(term.num_args() > 0){
    switch(func_kind(term)){
      case Z3_OP_ADD:
      case Z3_OP_SUB:
      case Z3_OP_UMINUS:
      case Z3_OP_MUL:
        {
          z3::expr_vector queue_args(sig.ctx);
          for(unsigned i = 0; i < term.num_args(); i++)
            queue_args.push_back(term.arg(i));

          while(queue_args.size() != 0){
            z3::expr const & curr = queue_args.back();
            queue_args.pop_back();
            if(curr.num_args() == 0)
              updateIndexVars(curr, side);
            else{
              for(unsigned i = 0; i < curr.num_args(); i++)
                queue_args.push_back(curr.arg(i));
            }
          }
        }
        return;
      case Z3_OP_UNINTERPRETED:
        for(unsigned i = 0; i < term.num_args(); i++){
          auto const & curr_arg = term.arg(i);
          auto const & type_arg = _get_sort(curr_arg);
          if(curr_arg.num_args() > 0)
            cojoin(
                curr_arg, 
                fresh_constant(type_arg), 
                side,
                current_conjs_in_input);
          else
            updateVarsDB(curr_arg, side); 
        }
        return;
      default:
        throw 
          "Problem @ "
          "Preprocessor::flattenTerm "
          "Invalid function aplication";
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

  z3::expr_vector temp_predicates(sig.ctx);
  z3::expr_vector from(sig.ctx), to(sig.ctx);
  from.push_back(old_term);
  to.push_back(new_const);

  switch(side){
    case PART_A:
      SUBSTITUTE_AND_ADJOINT_ONE(input_part_a);
      return;
    case PART_B:
      SUBSTITUTE_AND_ADJOINT_ONE(input_part_b);
      return;
  }
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
      if(!e.is_numeral())
        part_b_index_vars.push(e);
      return;
  }
}

void Preprocessor::updateVarsDB(
    z3::expr const & e, 
    SideInterpolant side){
  if(e.is_int()){
    updateIndexVars(e, side);
    return;
  }
  if(sig.isArraySort(e.get_sort())){
    updateArrayVars(e, side);
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
