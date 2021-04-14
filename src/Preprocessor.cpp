#include "Preprocess.h"
#include "z3++.h"

#define NORMALIZE_INPUT(OLD, NEW, TEMP_VAR)\
  z3::expr const & TEMP_VAR =\
  normalizeInput(OLD);\
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
  // empty_array is a common symbol
  part_a_array_vars.insert(sig.empty_array);
  part_b_array_vars.insert(sig.empty_array);

  unsigned current_conjs_in_input(0);
  FLATTEN_PREDICATE(current_conjs_in_input, input_part_a, PART_A);
  FLATTEN_PREDICATE(current_conjs_in_input, input_part_b, PART_B);

#if _DEBUG_PREPROCESS_
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
}

z3::expr Preprocessor::normalizeInputDefault(z3::expr const & e){
  assert(e.num_args() > 0);

  z3::func_decl f_name = e.decl();

  z3::expr_vector expr_args(sig.ctx);
  for(unsigned i = 0; i < e.num_args(); ++i)
    expr_args.push_back(normalizeInput(e.arg(i)));

  switch(f_name.decl_kind()){
    case Z3_OP_EQ:
      return expr_args[0] == expr_args[1];
    case Z3_OP_DISTINCT:
      return expr_args[0] != expr_args[1];
    case Z3_OP_UNINTERPRETED:
      {
        z3::sort_vector sort_args(sig.ctx);
        for(unsigned i = 0; i < e.num_args(); ++i)
          sort_args.push_back(expr_args[i].get_sort());

        if(f_name.range().is_array()){
          z3::func_decl lifted_f_name = sig.ctx.function(
              f_name.to_string().c_str(), 
              sort_args, 
              sig.getArraySortBySort(f_name.range().array_range()));

          return lifted_f_name(expr_args);
        }
        else {
          z3::func_decl lifted_f_name = sig.ctx.function(
              f_name.to_string().c_str(), 
              sort_args, 
              f_name.range());

          return lifted_f_name(expr_args);
        }

      }
    default:
      return f_name(expr_args);
  }
}

z3::expr Preprocessor::normalizeInput(z3::expr const & e){
  if(e.is_app())
    switch(e.num_args()){
      case 0:
        if(e.is_array())
          return sig.ctx.constant(func_name(e).c_str(), 
              sig.getArraySortBySort(e.get_sort().array_range()));
        return e;
      case 1:
        // [TODO] This needs testing
        if(func_name(e).find("length") != std::string::npos){
          auto const & arg = e.arg(0);
          z3::func_decl const & curr_diff = sig.getDiffBySort(arg.get_sort().array_range());
          z3::expr const & curr_empty_array = sig.getEmptyArrayBySort(arg.get_sort().array_range());
          return curr_diff(normalizeInput(arg), curr_empty_array);
        }

        if(e.decl().decl_kind() == Z3_OP_NOT){
          z3::expr predicate = e.arg(0);
          switch(predicate.decl().decl_kind()){
            case Z3_OP_EQ:       // ==
              return normalizeInput(predicate.arg(0)) 
                != normalizeInput(predicate.arg(1));
            case Z3_OP_DISTINCT: // !=
              return normalizeInput(predicate.arg(0)) 
                == normalizeInput(predicate.arg(1));
            case Z3_OP_GE:       // >=
              return normalizeInput(predicate.arg(0)) 
                < normalizeInput(predicate.arg(1));
            case Z3_OP_LE:       // <=
              return normalizeInput(predicate.arg(0)) 
                > normalizeInput(predicate.arg(1));
            case Z3_OP_GT:       // >
              return normalizeInput(predicate.arg(0)) 
                <= normalizeInput(predicate.arg(1));
            case Z3_OP_LT:       // <
              return normalizeInput(predicate.arg(0)) 
                >= normalizeInput(predicate.arg(1));
            case Z3_OP_UNINTERPRETED:
              if(predicate.get_sort().is_bool()){
                return normalizeInput(predicate) 
                  == sig.ctx.bool_val(false);
              }
            default:
              throw 
                "Error @ Preprocessor::normalizeInput. " 
                "Not is applied to a non predicate.";
          }
        }
        return normalizeInputDefault(e);

      case 2:
        // [TODO] This needs testing
        if(func_name(e).find("select") != std::string::npos){
          auto const & array_arg = e.arg(0);
          auto const & index_arg = e.arg(1);
          auto const & curr_rd = sig.getRdBySort(array_arg.get_sort().array_range());
          return curr_rd(normalizeInput(array_arg), index_arg);
        }
        if(func_name(e).find("diff") != std::string::npos){
          auto const & array_arg1 = e.arg(0);
          auto const & array_arg2 = e.arg(1);
          auto const & curr_diff = sig.getDiffBySort(array_arg1.get_sort().array_range());
          return curr_diff(normalizeInput(array_arg1), normalizeInput(array_arg2));
        }
        return normalizeInputDefault(e);

      case 3:
        // [TODO] This needs testing
        if(func_name(e).find("store") != std::string::npos){
          auto const & array_arg = e.arg(0);
          auto const & index_arg = e.arg(1);
          auto const & element_arg = e.arg(2);
          auto const & curr_wr = sig.getWrBySort(array_arg.get_sort().array_range());
          return curr_wr(normalizeInput(array_arg), index_arg, normalizeInput(element_arg));
        }
        return normalizeInputDefault(e);

      default:
        return normalizeInputDefault(e);
    }

  throw 
    "Problem @ "
    "Preprocessor::normalizeInput "
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
          updateVarsDB(
              lhs_form, 
              side);
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
  auto const & s_name = sort_name(e);
  if(s_name == sig.array_sort.name().str()){
    updateArrayVars(e, side);
    return;
  }
  if(s_name == sig.int_sort.name().str()){
    updateIndexVars(e, side);
    return;
  }
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

  throw 
    "Problem @ Preprocessor::fresh_constant. "
    "Given sort is not in the language";
}
