#include "Preprocess.h"

Preprocessor::Preprocessor(z3::context & ctx, char const * file):
  AXDSignature(ctx),
  really_a_parser(ctx), 
  fresh_index(0), current_conjs_in_input(0),
  input_part_a(ctx), 
  input_part_b(ctx),
  part_a_index_vars(ctx), part_b_index_vars(ctx),
  part_a_array_vars({}), part_b_array_vars({}), 
  common_array_vars({})
{

  really_a_parser.from_file(file);
  z3::expr_vector assertions = really_a_parser.assertions();
  assert(assertions.size() == 2);

  // empty_array is a common symbol
  part_a_array_vars.insert(empty_array);
  part_b_array_vars.insert(empty_array);

  // conjunction_a doesn't contain length
  // applications
  z3::expr const & conjunction_a = 
    removeLengthApplications(assertions[0]);
  for(unsigned i = 0; i < conjunction_a.num_args(); ++i)
    input_part_a.push_back(conjunction_a.arg(i));
  // conjunction_b doesn't contain length
  // applications
  z3::expr const & conjunction_b = 
    removeLengthApplications(assertions[1]);
  for(unsigned i = 0; i < conjunction_b.num_args(); ++i)
    input_part_b.push_back(conjunction_b.arg(i));

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
  m_out << "Part a " << conjunction_a << std::endl;
  m_out << "Part b " << conjunction_b << std::endl;
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

z3::expr Preprocessor::removeLengthApplications(z3::expr const & e){
  if(e.is_app()){
    if(e.num_args() > 0 && func_name(e) == "length")
      return diff(removeLengthApplications(e.arg(0)), empty_array);

    z3::func_decl f_name = e.decl();
    z3::expr_vector args(ctx);
    for(unsigned i = 0; i < e.num_args(); ++i)
      args.push_back(removeLengthApplications(e.arg(i)));
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
      flattenTerm(lhs(formula), side);
      flattenTerm(rhs(formula), side);
      return;
    case Z3_OP_DISTINCT: // !=
    case Z3_OP_GE:       // >=
    case Z3_OP_LE:       // <=
    case Z3_OP_GT:       // >
    case Z3_OP_LT:       // <
      flattenPredicateAux(formula, side);
      return;
    default:
      throw "Error at "
        "Preprocessor::flattenPredicate(z3::expr const &)." 
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

  if(lhs(atomic_predicate).num_args() == 0){
    if(rhs(atomic_predicate).num_args() == 0){
      // Nothing to do
    }
    else{
      old_terms.push_back(rhs(atomic_predicate));
      fresh_consts.push_back(fresh_constant(rhs(atomic_predicate).decl().range()));
    }
  }
  else{
    if(rhs(atomic_predicate).num_args() == 0){
      old_terms.push_back(lhs(atomic_predicate));
      fresh_consts.push_back(fresh_constant(lhs(atomic_predicate).decl().range()));
    }
    else{
      old_terms.push_back(lhs(atomic_predicate));
      fresh_consts.push_back(fresh_constant(lhs(atomic_predicate).decl().range()));
      old_terms.push_back(rhs(atomic_predicate));
      fresh_consts.push_back(fresh_constant(rhs(atomic_predicate).decl().range()));
    }
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

z3::expr Preprocessor::fresh_constant(z3::sort const & s){
  auto const & s_name = s.to_string();

  if(s_name == array_sort.to_string())
    return fresh_array_constant();
  if(s_name == element_sort.to_string())
    return fresh_element_constant();
  if(s_name == index_sort.to_string())
    return fresh_index_constant();

  throw "Problem @ Preprocessor::fresh_constant."
    "Given sort is not in the language";
}
