#include "AXDSignature.h"
#include "Preprocess.h"

// TODO:
// Investigate `updateVarsDB`
// This method should only be
// used with arrays variables
// and occurring index variables
// not integers

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

// flattens a predicate expression
// using flattenTerm and flattenPredidcateAux
// as helper functions
void axdinterpolator::Preprocessor::flattenPredicate(
    z3::expr const &formula, SideInterpolant side,
    unsigned &current_conjs_in_input) {
#if _DEBUG_PREPROCESS_
  m_out << std::endl;
  m_out << "-) Formula " << formula << std::endl;
#endif
  switch (func_kind(formula)) {
  case Z3_OP_TRUE:
  case Z3_OP_FALSE:
    return;
  case Z3_OP_EQ: // ==
  {
    auto const &lhs_form = lhs(formula);
#if _DEBUG_PREPROCESS_
    m_out << std::endl;
    m_out << "-) Formula@Z3_OP_EQ " << formula << std::endl;
#endif
    if (lhs_form.num_args() == 0) {
      updateVarsDB(lhs_form, side);
      flattenTerm(rhs(formula), side, current_conjs_in_input);
    } else
      flattenBinaryPredicate(formula, side, current_conjs_in_input);
  }
    return;
  case Z3_OP_DISTINCT: // !=
  case Z3_OP_GE:       // >=
  case Z3_OP_LE:       // <=
  case Z3_OP_GT:       // >
  case Z3_OP_LT:       // <
    flattenBinaryPredicate(formula, side, current_conjs_in_input);
    return;
  case Z3_OP_UNINTERPRETED:
    assert(formula.get_sort().is_bool());
    flattenTerm(formula, side, current_conjs_in_input);
    return;
  default:
    throw "Error at "
	  "axdinterpolator::Preprocessor::flattenPredicate(z3::expr const &). "
	  "Formula not in AXD";
  }
}

// Takes a binary atomic predicate,
// i.e. B(x, y) and updates the correspondant
// input side with fresh constants a, b if x
// and y arent constants (respectively) obtaining
// B(x, y)[x->a, y->b] \land x = a \land y = b
void axdinterpolator::Preprocessor::flattenBinaryPredicate(
    z3::expr const &atomic_predicate, SideInterpolant side,
    unsigned &current_conjs_in_input) {
#if _DEBUG_PREPROCESS_
  m_out << std::endl;
  m_out << "-) I see " << atomic_predicate << std::endl;
#endif

  z3::expr_vector old_terms(sig.ctx);
  z3::expr_vector fresh_consts(sig.ctx);
  z3::expr_vector temp_predicates(sig.ctx);

  auto const &lhs_atom = lhs(atomic_predicate);
  if (lhs_atom.num_args() > 0) {
    old_terms.push_back(lhs_atom);
    if (func_name(lhs_atom).find("length") != std::string::npos) {
      // This *should be fine* because every
      // array constant is given a bound
      // @ Preprocessor::normalizeInput
      fresh_consts.push_back(getLengthIndexVar(lhs_atom.arg(0)));
    } else {
      fresh_consts.push_back(fresh_constant(_get_sort(lhs_atom)));
    }
  }
  auto const &rhs_atom = rhs(atomic_predicate);
  if (rhs_atom.num_args() > 0) {
    old_terms.push_back(rhs_atom);
    if (func_name(rhs_atom).find("length") != std::string::npos) {
      // This *should be fine* because every
      // array constant is given a bound
      // @ Preprocessor::normalizeInput
      fresh_consts.push_back(getLengthIndexVar(rhs_atom.arg(0)));
    } else {
      fresh_consts.push_back(fresh_constant(_get_sort(rhs_atom)));
    }
  }

  switch (side) {
  case PART_A:
    SUBSTITUTE_AND_ADJOINT(input_part_a);
    return;
  case PART_B:
    SUBSTITUTE_AND_ADJOINT(input_part_b);
    return;
  }
}

void axdinterpolator::Preprocessor::flattenTerm(
    z3::expr const &term, SideInterpolant side,
    unsigned &current_conjs_in_input) {
  if (term.num_args() > 0) {
    switch (func_kind(term)) {
    case Z3_OP_UMINUS:
#if DETECT_THEORY
      if (sig.getTheoryName() < AXDSignature::QF_IDL)
	sig.setTheory(AXDSignature::QF_IDL);
#endif
    case Z3_OP_ADD:
    case Z3_OP_SUB:
    case Z3_OP_MUL:
    case Z3_OP_DIV:
    case Z3_OP_IDIV:
    case Z3_OP_REM:
    case Z3_OP_MOD:
#if DETECT_THEORY
      if (sig.getTheoryName() < AXDSignature::QF_LIA)
	sig.setTheory(AXDSignature::QF_LIA);
#endif
      {
	z3::expr_vector stack_args(sig.ctx);
	for (unsigned i = 0; i < term.num_args(); i++)
	  stack_args.push_back(term.arg(i));

	while (stack_args.size() != 0) {
	  z3::expr const &curr = stack_args.back();
	  stack_args.pop_back();
	  if (curr.num_args() == 0) {
	    updateVarsDB(curr, side);
	  } else {
	    for (unsigned i = 0; i < curr.num_args(); i++)
	      stack_args.push_back(curr.arg(i));
	  }
	}
      }
      return;
    case Z3_OP_UNINTERPRETED:
      for (unsigned i = 0; i < term.num_args(); i++) {
	auto term_name = func_name(term);
	if (term_name.find("wr") != std::string::npos) {
	  auto index_constant = term.arg(1);
	  switch (side) {
	  case PART_A:
	    if (indexALocalIds.find(index_constant.id()) ==
		indexALocalIds.end()) {
	      indexALocalIds.insert(index_constant.id());
	      n_IndexALocal++;
	    }
	    break;
	  case PART_B:
	    if (indexBLocalIds.find(index_constant.id()) ==
		indexBLocalIds.end()) {
	      indexBLocalIds.insert(index_constant.id());
	      n_IndexBLocal++;
	    }
	    break;
	  }
	}
	auto const &curr_arg = term.arg(i);
	auto const &type_arg = _get_sort(curr_arg);
	if (curr_arg.num_args() > 0)
	  cojoin(curr_arg, fresh_constant(type_arg), side,
		 current_conjs_in_input);
	else
	  updateVarsDB(curr_arg, side);
      }
      return;
    default:
      throw "Problem @ "
	    "axdinterpolator::Preprocessor::flattenTerm "
	    "Invalid function aplication";
    }
  } else
    updateVarsDB(term, side);
}

void axdinterpolator::Preprocessor::cojoin(z3::expr const &old_term,
					   z3::expr const &new_const,
					   SideInterpolant side,
					   unsigned &current_conjs_in_input) {

  z3::expr_vector temp_predicates(sig.ctx);
  z3::expr_vector from(sig.ctx), to(sig.ctx);
  from.push_back(old_term);
  to.push_back(new_const);

  switch (side) {
  case PART_A:
    SUBSTITUTE_AND_ADJOINT_ONE(input_part_a);
    return;
  case PART_B:
    SUBSTITUTE_AND_ADJOINT_ONE(input_part_b);
    return;
  }
}

void axdinterpolator::Preprocessor::updateArrayVars(z3::expr const &e,
						    SideInterpolant side) {
  switch (side) {
  case PART_A:
    part_a_array_vars.insert(e);
    return;
  case PART_B:
    part_b_array_vars.insert(e);
    return;
  }
}

void axdinterpolator::Preprocessor::updateIndexVars(z3::expr const &e,
						    SideInterpolant side) {
  switch (side) {
  case PART_A:
    part_a_index_vars.push(e);
    return;
  case PART_B:
    part_b_index_vars.push(e);
    return;
  }
}

void axdinterpolator::Preprocessor::updateVarsDB(z3::expr const &e,
						 SideInterpolant side) {
  if (e.is_int() && !e.is_numeral()) {
    updateIndexVars(e, side);
    return;
  }
  if (isArraySort(e.get_sort())) {
    updateArrayVars(e, side);
    return;
  }
}

z3::expr axdinterpolator::Preprocessor::fresh_index_constant() {
  return sig.ctx.constant(
      (FRESH_INDEX_PREFIX + std::to_string(fresh_num++)).c_str(), sig.int_sort);
}

z3::expr
axdinterpolator::Preprocessor::fresh_array_constant(z3::sort const &s) {
  return sig.ctx.constant(
      (FRESH_ARRAY_PREFIX + std::to_string(fresh_num++)).c_str(),
      sig.getArraySortBySort(s));
}

z3::expr axdinterpolator::Preprocessor::fresh_constant(z3::sort const &s) {
  if (isArraySort(s))
    return fresh_array_constant(s);
  if (s.is_int())
    return fresh_index_constant();

  throw "Problem @ axdinterpolator::Preprocessor::fresh_constant. "
	"No support for the given sort.";
}
