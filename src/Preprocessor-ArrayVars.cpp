#include "Preprocess.h"

Preprocessor::ArrayVars::ArrayVars() :
  vars({}), 
  size(0)
{ 
}

void Preprocessor::ArrayVars::insert(
    z3::expr const & array_var){
  unsigned index = array_var.get_sort().id();
  unsigned old_entry_size, new_entry_size;
  if(vars.find(index) == vars.end())
    vars.insert(
        std::pair<
        unsigned, 
        AXDSignature::z3_expr_set>(
          index, 
          AXDSignature::z3_expr_set({})));
  old_entry_size = vars[index].size();
  vars[index].insert(array_var);
  new_entry_size = vars[index].size();
  size += new_entry_size - old_entry_size;
}

bool Preprocessor::ArrayVars::isEmpty() const {
  return (size == 0);
}

unsigned Preprocessor::ArrayVars::getSize() const {
  return size;
}

Preprocessor::ArrayVars::Container::const_iterator Preprocessor::ArrayVars::begin() const {
  return vars.begin();
}

Preprocessor::ArrayVars::Container::const_iterator Preprocessor::ArrayVars::end() const {
  return vars.end();
}

std::ostream & operator << (std::ostream & os, 
    Preprocessor::ArrayVars const & vars){
  for(auto const & entry : vars){
    os << "Id of sort: ";
    os << entry.first << std::endl;
    for(auto const & x : entry.second)
      os << x << " ";
    os << std::endl;
  }
  return os;
}
