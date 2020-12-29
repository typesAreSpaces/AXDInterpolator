#include "AXDInterpolant.h"

z3::expr AXDInterpolant::QF_TO_Rewriter(z3::expr const & e){
  if(e.is_app()){
    if(e.num_args() > 0){
      switch(e.decl().decl_kind()){
        case Z3_OP_GE:
#if _DEBUG_QF_TO_REWRITER
          std::cout << "Testing 1" << std::endl;
          std::cout << e << std::endl;
          std::cout << ((e.arg(0) - e.arg(1)).simplify()) << std::endl;
          std::cout << QF_TO_RewriterAux((e.arg(0) - e.arg(1)).simplify()) << std::endl;
#endif
          return QF_TO_RewriterAux((e.arg(0) - e.arg(1)).simplify());
        case Z3_OP_LE:
#if _DEBUG_QF_TO_REWRITER
          std::cout << "Testing 2" << std::endl;
          std::cout << e << std::endl;
          std::cout << ((e.arg(1) - e.arg(0)).simplify()) << std::endl;
          std::cout << QF_TO_RewriterAux((e.arg(1) - e.arg(0)).simplify()) << std::endl;
#endif
          return QF_TO_RewriterAux((e.arg(1) - e.arg(0)).simplify());
        case Z3_OP_GT:
#if _DEBUG_QF_TO_REWRITER
          std::cout << "Testing 3" << std::endl;
          std::cout << e << std::endl;
          std::cout << ((e.arg(0) - e.arg(1) - 1).simplify()) << std::endl;
          std::cout << QF_TO_RewriterAux((e.arg(0) - e.arg(1) - 1).simplify()) << std::endl;
#endif
          return QF_TO_RewriterAux((e.arg(0) - e.arg(1) - 1).simplify());
        case Z3_OP_LT:
#if _DEBUG_QF_TO_REWRITER
          std::cout << "Testing 4" << std::endl;
          std::cout << e << std::endl;
          std::cout << ((e.arg(1) - e.arg(0) - 1).simplify()) << std::endl;
          std::cout << QF_TO_RewriterAux((e.arg(1) - e.arg(0) - 1).simplify()) << std::endl;
#endif
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
  if(normal_rhs.decl().decl_kind() != Z3_OP_ADD){
    if(normal_rhs.num_args() == 0)
      return 0 <= normal_rhs;
    return normal_rhs.arg(1) <= 0;
  }
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
    auto const & curr_arg = normal_rhs.arg(i);
    if(curr_arg.num_args() == 0 && !curr_arg.is_numeral()){
      is_def_pos_term = true;
      pos_term = curr_arg;
    }
    // A negative variable -x
    // is normalized to the product 
    // of -1 and x
    if(curr_arg.decl().decl_kind() == Z3_OP_MUL){
      is_def_neg_term = true;
      neg_term = curr_arg.arg(1);
    }
    if(curr_arg.is_numeral()){
      // If the constant term is zero 
      // the simplify command should have ignored it
      // Hence we just check positivity
      if(curr_arg.get_numeral_int() > 0)
        throw "Error @ AXDInterpolant::QF_TO_Rewriter"
          "The constant term is not negative";
      is_def_const_term = true;
    }
  }

  if(is_def_pos_term){
    if(is_def_neg_term){
      if(is_def_const_term){
        // 111
#if _DEBUG_QF_TO_REWRITER
        std::cout << "Processed as 111" << std::endl;
#endif
        return neg_term < pos_term;
      }

      else{
        // 110
#if _DEBUG_QF_TO_REWRITER
        std::cout << "Processed as 110" << std::endl;
#endif
        return neg_term <= pos_term;
      }
    }
    else{
      if(is_def_const_term){
        // 101
#if _DEBUG_QF_TO_REWRITER
        std::cout << "Processed as 101" << std::endl;
#endif
        return 0 < pos_term;
      }
      else{
        // 100
#if _DEBUG_QF_TO_REWRITER
        std::cout << "Processed as 100" << std::endl;
#endif
        return 0 <= pos_term;
      }
    }
  }
  else{
    if(is_def_neg_term){
      if(is_def_const_term){
        // 011
#if _DEBUG_QF_TO_REWRITER
        std::cout << "Processed as 011" << std::endl;
#endif
        return neg_term < 0;
      }
      else{
        // 010
#if _DEBUG_QF_TO_REWRITER
        std::cout << "Processed as 010" << std::endl;
#endif
        return neg_term <= 0;
      }
    }
    else{
      if(is_def_const_term){
        // 001
#if _DEBUG_QF_TO_REWRITER
        std::cout << "Processed as 001" << std::endl;
#endif
        return ctx.bool_val(false);
      }
      else{
        // 000
#if _DEBUG_QF_TO_REWRITER
        std::cout << "Processed as 000" << std::endl;
#endif
        return ctx.bool_val(true);
      }
    }
  }
}
