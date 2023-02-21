#include "util.h"

bool axdinterpolator::isSpaceOrParen(char T){
  return T == ' ' || T == '(' || T == ')';
}

void axdinterpolator::extractNameFromSort(std::string & s) {
  s.erase(
      std::remove_if(s.begin(), s.end(), isSpaceOrParen), 
      s.end());
}

std::ostream & operator << (
    std::ostream & out, 
    axdinterpolator::z3_expr_set const & collection){
  for(auto const & x : collection)
    out << x << std::endl;
  return out;
}

std::ostream & operator << (
    std::ostream & os, 
    std::map<unsigned, axdinterpolator::z3_expr_set> const & m){
  return os;
}
