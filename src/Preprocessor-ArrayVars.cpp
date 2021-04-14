#include "Preprocess.h"

Preprocessor::ArrayVars::ArrayVars() :
  vars({}) 
{ 
}

void Preprocessor::ArrayVars::insert(
    z3::expr const & array_var){
  unsigned index = array_var.get_sort().id();
  if(vars.find(index) == vars.end())
    vars.insert(
        std::pair<
        unsigned, 
        AXDSignature::z3_expr_set>(
          index, 
          AXDSignature::z3_expr_set({})));
  vars[index].insert(array_var);
}

Preprocessor::ArrayVars::Container::const_iterator Preprocessor::ArrayVars::begin() const {
  return vars.begin();
}

Preprocessor::ArrayVars::Container::const_iterator Preprocessor::ArrayVars::end() const {
  return vars.end();
}

std::ostream & operator << (std::ostream & os, 
    Preprocessor::ArrayVars const & vars){
  for(auto const & entry : vars.vars){
    os << "Id of sort: ";
    os << entry.first << std::endl;
    for(auto const & x : entry.second)
      os << x << " ";
    os << std::endl;
  }
  return os;
}
