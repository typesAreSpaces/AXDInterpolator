#include "Preprocess.h"

axdinterpolator::Preprocessor::ArrayVars::ArrayVars() :
  vars({}), 
  size(0)
{ 
}

void axdinterpolator::Preprocessor::ArrayVars::insert(
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

bool axdinterpolator::Preprocessor::ArrayVars::isEmpty() const {
  return (size == 0);
}

unsigned axdinterpolator::Preprocessor::ArrayVars::getSize() const {
  return size;
}

bool axdinterpolator::Preprocessor::ArrayVars::areCommonPairsAvaible() const {
  for(auto const & entry : vars){
    if(entry.second.size() >= 2)
      return true;
  }
  return false;
}

axdinterpolator::Preprocessor::ArrayVars::Container::const_iterator axdinterpolator::Preprocessor::ArrayVars::begin() const {
  return vars.begin();
}

axdinterpolator::Preprocessor::ArrayVars::Container::const_iterator axdinterpolator::Preprocessor::ArrayVars::end() const {
  return vars.end();
}

std::ostream & operator << (std::ostream & os, 
    axdinterpolator::Preprocessor::ArrayVars const & vars){
  for(auto const & entry : vars){
    os << "Id of sort: ";
    os << entry.first << std::endl;
    for(auto const & x : entry.second)
      os << x << " ";
    os << std::endl;
  }
  return os;
}
