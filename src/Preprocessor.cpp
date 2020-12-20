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

  // DEBUGGING
  m_out << "WTF?" << std::endl;

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

void Preprocessor::flattenPredicateAux(
    z3::expr const & formula, SideInterpolant side){

  z3::expr_vector from(ctx), to(ctx), temp_predicates(ctx);

  if(lhs(formula).num_args() == 0){
    if(rhs(formula).num_args() == 0){
    }
    else{
      from.push_back(rhs(formula));
      to.push_back(fresh_constant(rhs(formula).decl().range()));
    }
  }
  else{
    if(rhs(formula).num_args() == 0){
      from.push_back(lhs(formula));
      to.push_back(fresh_constant(lhs(formula).decl().range()));
    }
    else{
      from.push_back(lhs(formula));
      to.push_back(fresh_constant(lhs(formula).decl().range()));
      from.push_back(rhs(formula));
      to.push_back(fresh_constant(rhs(formula).decl().range()));
    }
  }

  switch(side){
    case PART_A:
      {
        for(unsigned i = 0; i < input_part_a.size(); i++)
          temp_predicates.push_back(
              input_part_a[i].substitute(from, to)
              );

        unsigned j = from.size();
        while(j--)
          temp_predicates.push_back(from[j] == to[j]);

        input_part_a = temp_predicates;
        current_conjs_in_input += from.size();
        return;
      }
    case PART_B:
      {
        for(unsigned i = 0; i < input_part_b.size(); i++)
          temp_predicates.push_back(
              input_part_b[i].substitute(from, to)
              );

        unsigned j = from.size();
        while(j--)
          temp_predicates.push_back(from[j] == to[j]);

        input_part_b = temp_predicates;
        current_conjs_in_input += from.size();
      }
      return;
  }
}

void Preprocessor::flattenTerm(z3::expr const & term, 
    SideInterpolant side){
  if(term.num_args() > 0){
    auto f_name = func_name(term);
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
    if(f_name == "pred"){
      if(term.arg(0).num_args() > 0)
        cojoin(term.arg(0), fresh_index_constant(), side);
      else
        updateIndexVars(term.arg(0), side);
      return;
    }
    if(f_name == "succ"){
      if(term.arg(0).num_args() > 0)
        cojoin(term.arg(0), fresh_index_constant(), side);
      else
        updateIndexVars(term.arg(0), side);
      return;
    }
    if(f_name == "neg"){
      if(term.arg(0).num_args() > 0)
        cojoin(term.arg(0), fresh_index_constant(), side);
      else
        updateIndexVars(term.arg(0), side);
      return;
    }
    if(f_name == "add"){
      if(term.arg(0).num_args() > 0)
        cojoin(term.arg(0), fresh_index_constant(), side);
      else
        updateIndexVars(term.arg(0), side);
      if(term.arg(1).num_args() > 0)
        cojoin(term.arg(1), fresh_index_constant(), side);
      else
        updateIndexVars(term.arg(1), side);
      return;
    }
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
    return fresh_index_constant();
  if(s_name == index_sort.to_string())
    return fresh_index_constant();
  throw "Problem @ Preprocessor::fresh_constant."
    "Given sort is not in the language";
}
