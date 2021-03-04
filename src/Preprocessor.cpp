#include "Preprocess.h"

Preprocessor::Preprocessor(
    z3::context & ctx, 
    z3::expr const & assertions):
  AXDSignature(ctx),
  current_conjs_in_input(0),
  fresh_index(0), 
  input_part_a(ctx), 
  input_part_b(ctx),
  part_a_index_vars(ctx), part_b_index_vars(ctx),
  part_a_array_vars({}), part_b_array_vars({}), 
  common_array_vars({})
{
  // NOTE:
  // For now, it is assumed that
  // assertions is a conjunction of two 
  // conjunctions i.e. Part-A and Part-B
  // are both conjunctions
  assert(assertions.num_args() == 2);

  // empty_array is a common symbol
  part_a_array_vars.insert(empty_array);
  part_b_array_vars.insert(empty_array);

  // conjunction_a doesn't contain length
  // applications
  z3::expr const & conjunction_a = 
    remove_Not_Length_Apps(assertions.arg(0));
  for(unsigned i = 0; i < conjunction_a.num_args(); ++i){
    auto const & curr_arg = conjunction_a.arg(i);
    if(curr_arg.decl().decl_kind() == Z3_OP_EQ
        && rhs(curr_arg).num_args() == 0)
      input_part_a.push_back(rhs(curr_arg) == lhs(curr_arg));
    else
      input_part_a.push_back(curr_arg);
  }
  // conjunction_b doesn't contain length
  // applications
  z3::expr const & conjunction_b = 
    remove_Not_Length_Apps(assertions.arg(1));
  for(unsigned i = 0; i < conjunction_b.num_args(); ++i){
    auto const & curr_arg = conjunction_b.arg(i);
    if(curr_arg.decl().decl_kind() == Z3_OP_EQ
        && rhs(curr_arg).num_args() == 0)
      input_part_b.push_back(rhs(curr_arg) == lhs(curr_arg));
    else
      input_part_b.push_back(curr_arg);
  }

#if _DEBUG_PREPROCESS_
  m_out << "Conjunction a" << std::endl;
  m_out << conjunction_a << std::endl;
  m_out << "Conjunction b" << std::endl;
  m_out << conjunction_b << std::endl;
#endif

  // Processing Part-A
  current_conjs_in_input = input_part_a.size();
  for(unsigned i = 0; i < current_conjs_in_input; i++)
    flattenPredicate(input_part_a[i], PART_A);

  // Processing Part-B
  current_conjs_in_input = input_part_b.size();
  for(unsigned i = 0; i < current_conjs_in_input; i++)
    flattenPredicate(input_part_b[i], PART_B);

  // Set current_conjs_in_input to zero
  // because the variable is no longer needed
  current_conjs_in_input = 0;

#if _DEBUG_PREPROCESS_
  m_out << "Assertions " << assertions << std::endl;
  m_out << "Part a " << input_part_a << std::endl;
  m_out << "Part b " << input_part_b << std::endl;
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

#if _DEBUG_PREPROCESS_
  m_out << "Before" << std::endl;
  m_out << part_a_index_vars << std::endl;
  m_out << part_b_index_vars << std::endl;
#endif

  removeDuplicates(part_a_index_vars);
  removeDuplicates(part_b_index_vars);

#if _DEBUG_PREPROCESS_
  m_out << "After" << std::endl;
  m_out << part_a_index_vars << std::endl;
  m_out << part_b_index_vars << std::endl;
#endif
}

z3::expr Preprocessor::remove_Not_Length_Apps(z3::expr const & e){
  if(e.is_app()){
    if(e.num_args() > 0 && func_name(e) == "length")
      return diff(
          remove_Not_Length_Apps(e.arg(0)), 
          empty_array);

    if(e.num_args() == 1 && e.decl().decl_kind() == Z3_OP_NOT){
      z3::expr pred = e.arg(0);
      switch(pred.decl().decl_kind()){
        case Z3_OP_EQ:       // ==
          return remove_Not_Length_Apps(pred.arg(0)) 
            != remove_Not_Length_Apps(pred.arg(1));
        case Z3_OP_DISTINCT: // !=
          return remove_Not_Length_Apps(pred.arg(0)) 
            == remove_Not_Length_Apps(pred.arg(1));
        case Z3_OP_GE:       // >=
          return remove_Not_Length_Apps(pred.arg(0)) 
            < remove_Not_Length_Apps(pred.arg(1));
        case Z3_OP_LE:       // <=
          return remove_Not_Length_Apps(pred.arg(0)) 
            > remove_Not_Length_Apps(pred.arg(1));
        case Z3_OP_GT:       // >
          return remove_Not_Length_Apps(pred.arg(0)) 
            <= remove_Not_Length_Apps(pred.arg(1));
        case Z3_OP_LT:       // <
          return remove_Not_Length_Apps(pred.arg(0)) 
            >= remove_Not_Length_Apps(pred.arg(1));
        case Z3_OP_UNINTERPRETED:
          if(pred.get_sort().to_string() 
              == bool_sort.to_string()){
            return remove_Not_Length_Apps(pred) 
              == ctx.bool_val(false);
          }
        default:
          throw "Error @ Preprocessor::remove_Not_Legth_Apps." 
            "Not is applied to a non predicate.";
      }
    }

    if(e.num_args() == 0)
      return e;

    z3::func_decl f_name = e.decl();
    z3::expr_vector args(ctx);
    for(unsigned i = 0; i < e.num_args(); ++i)
      args.push_back(remove_Not_Length_Apps(e.arg(i)));

    return f_name(args);
  }
  throw "Problem @ "
    "Preprocessor::removeLengthApplications" 
    "Not an application";
}

void Preprocessor::flattenPredicate(
    z3::expr const & formula, 
    SideInterpolant side){
  switch(formula.decl().decl_kind()){
    case Z3_OP_EQ:       // ==
      {
        auto const & lhs_form = lhs(formula);
        if(lhs_form.num_args() == 0){
          updateVarsDB(lhs_form, 
              lhs_form.decl().range(), 
              side);
          flattenTerm(rhs(formula), side);
        }
        else
          flattenPredicateAux(formula, side);
      }
      return;
    case Z3_OP_DISTINCT: // !=
    case Z3_OP_GE:       // >=
    case Z3_OP_LE:       // <=
    case Z3_OP_GT:       // >
    case Z3_OP_LT:       // <
      flattenPredicateAux(formula, side);
      return;
    case Z3_OP_UNINTERPRETED:
      if(formula.get_sort().to_string() 
          == bool_sort.to_string()){
        flattenTerm(formula, side);
        return;
      }
    default:
      throw "Error at "
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
    z3::expr const & atomic_predicate, SideInterpolant side){

  z3::expr_vector old_terms(ctx); 
  z3::expr_vector fresh_consts(ctx);
  z3::expr_vector temp_predicates(ctx);

  auto const & lhs_atom = lhs(atomic_predicate);
  if(lhs_atom.num_args() > 0){
    old_terms.push_back(lhs_atom);
    fresh_consts.push_back(
        fresh_constant(lhs_atom.decl().range()));
  }
  auto const & rhs_atom = rhs(atomic_predicate);
  if(rhs_atom.num_args() > 0){
    old_terms.push_back(rhs_atom);
    fresh_consts.push_back(
        fresh_constant(rhs_atom.decl().range()));
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
    SideInterpolant side){
  if(term.num_args() > 0){
    auto f_name = func_name(term);
    for(unsigned i = 0; i < term.num_args(); i++){
      auto const & curr_arg = term.arg(i);
      auto const & type_arg = curr_arg.decl().range();
      if(curr_arg.num_args() > 0)
        cojoin(curr_arg, fresh_constant(type_arg), side);
      else
        updateVarsDB(curr_arg, type_arg, side); 
    }
  }
  else
    updateVarsDB(term, term.decl().range(), side);
}

void Preprocessor::cojoin(
    z3::expr const & old_term, 
    z3::expr const & new_const, 
    SideInterpolant side){
  switch(side){
    case PART_A:
      cojoin_aux(input_part_a, old_term, new_const);
      return;
    case PART_B:
      cojoin_aux(input_part_b, old_term, new_const);
      return;
  }
}

void Preprocessor::cojoin_aux(
    z3::expr_vector & predicates,
    z3::expr const & old_term, 
    z3::expr const & new_const){

  z3::expr_vector from(ctx), to(ctx);
  from.push_back(old_term);
  to.push_back(new_const);

  z3::expr_vector temp_predicates(ctx);
  for(unsigned i = 0; i < predicates.size(); i++)
    temp_predicates.push_back(
        predicates[i].substitute(from, to));
  temp_predicates.push_back(new_const == old_term);
  predicates = temp_predicates;
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

void Preprocessor::updateVarsDB(z3::expr const & e, 
    z3::sort const & s, SideInterpolant side){
  auto const & s_name = s.name().str();
  if(s_name == array_sort.name().str()){
    updateArrayVars(e, side);
    return;
  }
  if(s_name == index_sort.name().str()){
    updateIndexVars(e, side);
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

z3::expr Preprocessor::fresh_bool_constant(){
  return ctx.constant((FRESH_INDEX_PREFIX 
        + std::to_string(fresh_index++)).c_str(), bool_sort);
}

z3::expr Preprocessor::fresh_index_constant(){
  return ctx.constant((FRESH_INDEX_PREFIX 
        + std::to_string(fresh_index++)).c_str(), int_sort);
}

z3::expr Preprocessor::fresh_element_constant(){
  return ctx.constant((FRESH_ELEMENT_PREFIX 
        + std::to_string(fresh_index++)).c_str(), element_sort);
}

z3::expr Preprocessor::fresh_array_constant(){
  return ctx.constant((FRESH_ARRAY_PREFIX 
        + std::to_string(fresh_index++)).c_str(), array_sort);
}

z3::expr Preprocessor::fresh_constant(z3::sort const & s){
  auto const & s_name = s.to_string();

  if(s_name == bool_sort.to_string())
    return fresh_bool_constant();
  if(s_name == array_sort.to_string())
    return fresh_array_constant();
  if(s_name == element_sort.to_string())
    return fresh_element_constant();
  if(s_name == index_sort.to_string())
    return fresh_index_constant();

  throw "Problem @ Preprocessor::fresh_constant."
    "Given sort is not in the language";
}
