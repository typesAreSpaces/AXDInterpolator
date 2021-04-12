#include "Preprocess.h"
#include "z3++.h"

#define NORMALIZE_INPUT(OLD, NEW, TEMP_VAR)\
  z3::expr const & TEMP_VAR =\
  initialTraverse(OLD);\
  for(unsigned i = 0; i < TEMP_VAR.num_args(); ++i){\
    auto const & curr_arg = TEMP_VAR.arg(i);\
    if(curr_arg.decl().decl_kind() == Z3_OP_EQ\
        && rhs(curr_arg).num_args() == 0)\
    NEW.push_back(rhs(curr_arg) == lhs(curr_arg));\
    else\
    NEW.push_back(curr_arg);\
  }

#define FLATTEN_PREDICATE(NUM_CONJS_INPUT, CONJUNCTION, SIDE)\
  NUM_CONJS_INPUT = CONJUNCTION.size();\
  for(unsigned i = 0; i < NUM_CONJS_INPUT; i++)\
  flattenPredicate(CONJUNCTION[i], SIDE, NUM_CONJS_INPUT);

Preprocessor::Preprocessor(
    AXDSignature & sig, 
    z3::expr const & _input_part_a, 
    z3::expr const & _input_part_b):
  sig(sig),
  fresh_index(0), 
  input_part_a(sig.ctx), 
  input_part_b(sig.ctx),
  part_a_index_vars(sig.ctx), 
  part_b_index_vars(sig.ctx),
  part_a_array_vars({}), 
  part_b_array_vars({}), 
  common_array_vars({})
{
  NORMALIZE_INPUT(_input_part_a, input_part_a, conjunction_a);
  NORMALIZE_INPUT(_input_part_b, input_part_b, conjunction_b);

#if _DEBUG_PREPROCESS_
  m_out << "Conjunction a" << std::endl;
  m_out << conjunction_a << std::endl;
  m_out << "Conjunction b" << std::endl;
  m_out << conjunction_b << std::endl;
#endif

  // [TODO]: parametrize empty_array elements
  //
  // empty_array is a common symbol
  part_a_array_vars.insert(sig.empty_array);
  part_b_array_vars.insert(sig.empty_array);

  unsigned current_conjs_in_input(0);
  FLATTEN_PREDICATE(current_conjs_in_input, input_part_a, PART_A);
  FLATTEN_PREDICATE(current_conjs_in_input, input_part_b, PART_B);

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

  removeDuplicates(part_a_index_vars);
  removeDuplicates(part_b_index_vars);
}

z3::expr Preprocessor::initialTraverse(z3::expr const & e){
  if(e.is_app()){
    // [TODO] parametrize the
    // detection of length applications
    if(e.num_args() == 1 
        && func_name(e) == "length")
      return sig.diff(
          initialTraverse(e.arg(0)), 
          sig.empty_array);

    if(e.num_args() == 1 
        && e.decl().decl_kind() == Z3_OP_NOT){
      z3::expr predicate = e.arg(0);
      switch(predicate.decl().decl_kind()){
        case Z3_OP_EQ:       // ==
          return initialTraverse(predicate.arg(0)) 
            != initialTraverse(predicate.arg(1));
        case Z3_OP_DISTINCT: // !=
          return initialTraverse(predicate.arg(0)) 
            == initialTraverse(predicate.arg(1));
        case Z3_OP_GE:       // >=
          return initialTraverse(predicate.arg(0)) 
            < initialTraverse(predicate.arg(1));
        case Z3_OP_LE:       // <=
          return initialTraverse(predicate.arg(0)) 
            > initialTraverse(predicate.arg(1));
        case Z3_OP_GT:       // >
          return initialTraverse(predicate.arg(0)) 
            <= initialTraverse(predicate.arg(1));
        case Z3_OP_LT:       // <
          return initialTraverse(predicate.arg(0)) 
            >= initialTraverse(predicate.arg(1));
        case Z3_OP_UNINTERPRETED:
          if(predicate.get_sort().is_bool()){
            return initialTraverse(predicate) 
              == sig.ctx.bool_val(false);
          }
        default:
          throw "Error @ Preprocessor::remove_Not_Legth_Apps." 
            "Not is applied to a non predicate.";
      }
    }

    if(e.num_args() == 0)
      return e;

    z3::func_decl f_name = e.decl();
    z3::expr_vector args(sig.ctx);
    for(unsigned i = 0; i < e.num_args(); ++i)
      args.push_back(initialTraverse(e.arg(i)));

    return f_name(args);
  }
  throw "Problem @ "
    "Preprocessor::initialTraverse" 
    "Not an application";
}

void Preprocessor::flattenPredicate(
    z3::expr const & formula, 
    SideInterpolant side,
    unsigned & current_conjs_in_input){
  switch(formula.decl().decl_kind()){
    case Z3_OP_EQ:       // ==
      {
        auto const & lhs_form = lhs(formula);
        if(lhs_form.num_args() == 0){
          updateVarsDB(lhs_form, 
              lhs_form.decl().range(), 
              side);
          flattenTerm(rhs(formula), side, 
              current_conjs_in_input);
        }
        else
          flattenPredicateAux(formula, side,
              current_conjs_in_input);
      }
      return;
    case Z3_OP_DISTINCT: // !=
    case Z3_OP_GE:       // >=
    case Z3_OP_LE:       // <=
    case Z3_OP_GT:       // >
    case Z3_OP_LT:       // <
      flattenPredicateAux(formula, side,
          current_conjs_in_input);
      return;
    case Z3_OP_UNINTERPRETED:
      if(formula.get_sort().is_bool()){
        flattenTerm(formula, side,
            current_conjs_in_input);
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
    z3::expr const & atomic_predicate, SideInterpolant side,
    unsigned & current_conjs_in_input){

  z3::expr_vector old_terms(sig.ctx); 
  z3::expr_vector fresh_consts(sig.ctx);
  z3::expr_vector temp_predicates(sig.ctx);

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
    SideInterpolant side,
    unsigned & current_conjs_in_input){
  if(term.num_args() > 0){
    auto f_name = func_name(term);
    for(unsigned i = 0; i < term.num_args(); i++){
      auto const & curr_arg = term.arg(i);
      auto const & type_arg = curr_arg.decl().range();
      if(curr_arg.num_args() > 0)
        cojoin(curr_arg, fresh_constant(type_arg), side,
            current_conjs_in_input);
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
  if(s_name == sig.array_sort.name().str()){
    updateArrayVars(e, side);
    return;
  }
  if(s_name == sig.int_sort.name().str()){
    updateIndexVars(e, side);
    return;
  }
}

void Preprocessor::removeDuplicates(z3::expr_vector & terms){
  z3::expr_vector new_terms(sig.ctx);
  AXDSignature::z3_expr_set ids({});
  for(auto const & term : terms)
    if(!inSet(term, ids)){
      ids.insert(term);
      new_terms.push_back(term);
    }

  terms = new_terms;
}

z3::expr Preprocessor::fresh_index_constant(){
  return sig.ctx.constant((FRESH_INDEX_PREFIX 
        + std::to_string(fresh_index++)).c_str(), sig.int_sort);
}

z3::expr Preprocessor::fresh_array_constant(){
  return sig.ctx.constant((FRESH_ARRAY_PREFIX 
        + std::to_string(fresh_index++)).c_str(), sig.array_sort);
}

z3::expr Preprocessor::fresh_constant(z3::sort const & s){
  auto const & s_name = s.to_string();

  // TODO: remove this one and use is_array()
  if(s_name == sig.array_sort.to_string())
    return fresh_array_constant();
  if(s.is_int())
    return fresh_index_constant();

  throw "Problem @ Preprocessor::fresh_constant."
    "Given sort is not in the language";
}
