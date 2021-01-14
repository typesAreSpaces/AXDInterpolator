#include "AXDInterpolant.h"

std::string AXDInterpolant::defineDeclarations(std::string decls) const {
  std::string pred_decl = "(declare-fun pred (Int) Int)";
  std::string succ_decl = "(declare-fun succ (Int) Int)";
  std::string neg_decl  = "(declare-fun neg (Int) Int)";
  std::string add_decl  = "(declare-fun add (Int Int) Int)";
  std::string result = decls;

  auto position = decls.find(pred_decl);
  if(position != std::string::npos)
    result = result.replace(position, pred_decl.size(), "(define-fun pred ((x Int)) Int (- x 1))");

  position = result.find(succ_decl);
  if(position != std::string::npos)
    result = result.replace(position, succ_decl.size(), "(define-fun succ ((x Int)) Int (+ x 1))");

  position = result.find(neg_decl);
  if(position != std::string::npos)
    result = result.replace(position, neg_decl.size(), "(define-fun neg ((x Int)) Int (- x))");

  position = result.find(add_decl);
  if(position != std::string::npos)
    result = result.replace(position, add_decl.size(), "(define-fun add ((x Int) (y Int)) Int (+ x y))");

  return result;
}
