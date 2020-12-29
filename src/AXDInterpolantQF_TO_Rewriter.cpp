#include "AXDInterpolant.h"

z3::expr AXDInterpolant::QF_TO_Rewriter(z3::expr const & e){
  if(e.is_app()){
    if(e.num_args() > 0){
      switch(e.decl().decl_kind()){
        case Z3_OP_GE:
          return QF_TO_RewriterAux((e.arg(0) - e.arg(1)).simplify());
        case Z3_OP_LE:
          return QF_TO_RewriterAux((e.arg(1) - e.arg(0)).simplify());
        case Z3_OP_GT:
          return QF_TO_RewriterAux((e.arg(0) - e.arg(1) - 1).simplify());
        case Z3_OP_LT:
          return QF_TO_RewriterAux((e.arg(1) - e.arg(0) - 1).simplify());
        default: 
          {
            z3::func_decl f_name = e.decl();
            z3::expr_vector args(ctx);
            for(unsigned i = 0; i < e.num_args(); ++i)
              args.push_back(QF_TO_Rewriter(e.arg(i)));
            return f_name(args);
          }
      }
    }
    return e;
  }
  else
    throw 
      "Error @ AXDInterpolant::QF_TO_Rewriter."
      "e is not a function application";
}

// This functions takes as input an expression
// of the form 0 <= normal_rhs
// and returns an expression in the language of
// QF_TO
z3::expr AXDInterpolant::QF_TO_RewriterAux(z3::expr const & normal_rhs){
  //auto const & normal_rhs = (e.arg(1) - e.arg(0)).simplify();
  auto const & n_rhs_num_args = normal_rhs.num_args();
  bool is_def_pos_term = false, 
       is_def_neg_term = false, 
       is_def_const_term = false;
  z3::expr pos_term(ctx), neg_term(ctx);
  if(n_rhs_num_args > 3)
    throw 
      "Error @ AXDInterpolant::QF_TO_Rewriter"
      "The normal right hand side has more than "
      "three terms";
  for(unsigned i = 0; i < normal_rhs.num_args(); ++i){
    if(normal_rhs.arg(i).num_args() == 0){
      is_def_pos_term = true;
      pos_term = normal_rhs.arg(i);
    }
    // A negative variable -x
    // is normalized to the product 
    // of -1 and x
    if(normal_rhs.arg(i).decl().decl_kind() == Z3_OP_MUL){
      is_def_neg_term = true;
      neg_term = normal_rhs.arg(i).arg(1);
    }
    if(normal_rhs.arg(i).is_numeral()){
      // If the constant term is zero 
      // the simplify command should have ignored it
      // Hence we just check positivity
      if(normal_rhs.arg(i).get_numeral_int() > 0)
        throw "Error @ AXDInterpolant::QF_TO_Rewriter"
          "The constant term is not negative";
      is_def_const_term = true;
    }
  }

  if(is_def_pos_term){
    if(is_def_neg_term){
      if(is_def_const_term)
        // 111
        return neg_term < pos_term;

      else
        // 110
        return neg_term <= pos_term;
    }
    else{
      if(is_def_const_term)
        // 101
        return 0 < pos_term;
      else
        // 100
        return 0 <= pos_term;
    }
  }
  else{
    if(is_def_neg_term){
      if(is_def_const_term)
        // 011
        return neg_term < 0;
      else
        // 010
        return neg_term <= 0;
    }
    else{
      if(is_def_const_term)
        // 001
        return ctx.bool_val(false);
      else
        // 000
        return ctx.bool_val(true);
    }
  }
}
