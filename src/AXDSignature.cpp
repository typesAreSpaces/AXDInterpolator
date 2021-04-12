#include "AXDSignature.h"

AXDSignature::AXDSignature(
    z3::context & ctx, 
    char const * theory_string) :
  ctx(ctx),

  bool_sort(ctx.bool_sort()), 
  int_sort(ctx.int_sort()), 

  // -------------------------------
  // [TODO] remove the following
  // Parametrize them instead with
  // every type A from (Array Int A)
  //element_sort(ctx.uninterpreted_sort("ElementSort")), 
  element_sort(ctx.int_sort()), 
  array_sort(ctx.uninterpreted_sort("ArraySort")),
  // -------------------------------

  element_sorts(ctx),
  undefined_es(ctx),
  empty_array_es(ctx),
  diff_es(ctx),
  diff_k_es(ctx),
  wr_es(ctx),
  rd_es(ctx),
  length_es(ctx),

  // -------------------------------
  // [TODO] remove the following
  // Parametrize them instead with
  // every type A from (Array Int A)
  //element_sort(ctx.uninterpreted_sort("ElementSort")), 
  undefined(ctx.constant("undefined", element_sort)),
  empty_array(ctx.constant("empty_array", array_sort)),

  diff(ctx.function("diff", array_sort, array_sort, int_sort)),
  diff_k(ctx.function("diff_", int_sort, array_sort, array_sort, int_sort)),
  wr(ctx.function("wr", array_sort, int_sort, element_sort, array_sort)),
  rd(ctx.function("rd", array_sort, int_sort, element_sort)),
  // length is a conservative extension of any Th(T_I)
  // because \forall x . length(x) = diff(x, empty_array)
  length(ctx.function("length", array_sort, int_sort))
  // -------------------------------
{
  if(!strcmp(theory_string, "QF_TO"))
    theory_name = QF_TO;
  else if(!strcmp(theory_string, "QF_IDL"))
    theory_name = QF_IDL;
  else if(!strcmp(theory_string, "QF_UTVPI"))
    theory_name = QF_UTVPI;
  else if(!strcmp(theory_string, "QF_LIA"))
    theory_name = QF_LIA;
  else
    throw "Error: theory not supported";
}

bool AXDSignature::isSpaceOrParen(char T){
  return T == ' ' || T == '(' || T == ')';
}

void AXDSignature::extractNameFromSort(std::string & s) const {
  s.erase(
      std::remove_if(s.begin(), s.end(), isSpaceOrParen), 
      s.end());
}

bool AXDSignature::is_QF_TO() const {
  return theory_name == QF_TO;
}

bool AXDSignature::is_QF_IDL() const {
  return theory_name == QF_IDL;
}

AXDSignature::TheoryName AXDSignature::getTheoryName() const {
  return theory_name;
}

std::ostream & operator << (
    std::ostream & out, 
    AXDSignature::z3_expr_set const & collection){
  for(auto const & x : collection)
    out << x << std::endl;
  return out;
}
