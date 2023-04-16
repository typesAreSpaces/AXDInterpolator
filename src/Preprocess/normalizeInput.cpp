#include "Preprocess.h"

// Lifts function names and signatures
// of terms involving arrays to abstract arrays
z3::expr axdinterpolator::Preprocessor::normalizeInputDefault(z3::expr const & e){
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
              func_name(e).c_str(),
              sort_args, 
              sig.getArraySortBySort(f_name.range().array_range()));

          return lifted_f_name(expr_args);
        }
        else {
          z3::func_decl lifted_f_name = sig.ctx.function(
              func_name(e).c_str(),
              sort_args, 
              f_name.range());

          return lifted_f_name(expr_args);
        }

      }
    default:
      return f_name(expr_args);
  }
}

// Rewrites :
// -) constant of type array into 'same' constant of type 'abstract array'
// -) (NOT (pred ... )) into (dual_pred ... )
// -) (select a i1) into (rd a i1) with abstract sort types
// -) (diff a b) into (diff a b) with abstract sort types
// -) (store a i e) into (wr a i e) with abstract sort types
// -) otherwise: rewrites using normalizeInputDefault
z3::expr axdinterpolator::Preprocessor::normalizeInput(z3::expr const &e) {
  if (e.is_app())
      switch (e.num_args()) {
      case 0:
	  if (e.is_array()) {
	    auto const &new_e = sig.ctx.constant(
		func_name(e).c_str(),
		sig.getArraySortBySort(e.get_sort().array_range()));
	    // Every array variable should have a bounded size
	    // i.e. for every a array, we include |a| = i for some i
	    updateLengthIndexVars(new_e, false);
	    return new_e;
	  }
	  return e;
      case 1:
	  // Length rewrite is disabled:
	  // The CAXD extension handles this case
	  // differently
#if 0
	  if (func_name(e).find("length") != std::string::npos) {
	    auto const &arg = e.arg(0);
	    z3::func_decl const &curr_diff =
		sig.getDiffBySort(arg.get_sort().array_range());
	    z3::expr const &curr_empty_array =
		sig.getEmptyArrayBySort(arg.get_sort().array_range());
	    return curr_diff(normalizeInput(arg), curr_empty_array);
	  }
#endif

	  if (func_kind(e) == Z3_OP_NOT) {
	    z3::expr predicate = e.arg(0);
	    switch (func_kind(predicate)) {
	    case Z3_OP_TRUE:
	      return sig.ctx.bool_val(false);
	    case Z3_OP_FALSE:
	      return sig.ctx.bool_val(true);
	    case Z3_OP_EQ: // ==
	      return normalizeInput(predicate.arg(0)) !=
		     normalizeInput(predicate.arg(1));
	    case Z3_OP_DISTINCT: // !=
	      return normalizeInput(predicate.arg(0)) ==
		     normalizeInput(predicate.arg(1));
	    case Z3_OP_GE: // >=
	      return normalizeInput(predicate.arg(0)) <
		     normalizeInput(predicate.arg(1));
	    case Z3_OP_LE: // <=
	      return normalizeInput(predicate.arg(0)) >
		     normalizeInput(predicate.arg(1));
	    case Z3_OP_GT: // >
	      return normalizeInput(predicate.arg(0)) <=
		     normalizeInput(predicate.arg(1));
	    case Z3_OP_LT: // <
	      return normalizeInput(predicate.arg(0)) >=
		     normalizeInput(predicate.arg(1));
	    case Z3_OP_UNINTERPRETED:
	      if (predicate.get_sort().is_bool()) {
		return normalizeInput(predicate) == sig.ctx.bool_val(false);
	      }
	    default:
	      throw "Error @ axdinterpolator::Preprocessor::normalizeInput. "
		    "Not is applied to a non predicate.";
	    }
	  }
	  return normalizeInputDefault(e);

      case 2:
	  if (func_name(e).find("select") != std::string::npos) {
	    auto const &array_arg = e.arg(0);
	    auto const &index_arg = e.arg(1);
	    auto const &curr_rd =
		sig.getRdBySort(array_arg.get_sort().array_range());
	    return curr_rd(normalizeInput(array_arg), index_arg);
	  }
	  if (func_name(e).find("diff") != std::string::npos) {
	    auto const &array_arg1 = e.arg(0);
	    auto const &array_arg2 = e.arg(1);
	    auto const &curr_diff =
		sig.getDiffBySort(array_arg1.get_sort().array_range());
	    return curr_diff(normalizeInput(array_arg1),
			     normalizeInput(array_arg2));
	  }
	  return normalizeInputDefault(e);

      case 3:
	  if (func_name(e).find("store") != std::string::npos) {
	    auto const &array_arg = e.arg(0);
	    auto const &index_arg = e.arg(1);
	    auto const &element_arg = e.arg(2);
	    auto const &curr_wr =
		sig.getWrBySort(array_arg.get_sort().array_range());
	    return curr_wr(normalizeInput(array_arg), index_arg,
			   normalizeInput(element_arg));
	  }
	  return normalizeInputDefault(e);

      default:
	  return normalizeInputDefault(e);
      }

  throw "Problem @ "
	"axdinterpolator::Preprocessor::normalizeInput "
	"Not an application";
}
