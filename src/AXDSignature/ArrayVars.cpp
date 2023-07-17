#include "AXDSignature.h"

axdinterpolator::ArrayVars::ArrayVars() :
  vars({}), 
  size(0)
{ 
}

void axdinterpolator::ArrayVars::insert(
    z3::expr const & array_var){
  unsigned index = array_var.get_sort().id();
#if _DEBUG_ARRAY_VARS 
  m_out << ">> Debugging axdinterpolator::ArrayVars" << std::endl;
  m_out << "Index: " << index << std::endl;
  m_out << "Array var: " << array_var << std::endl;
  m_out << "Sort of array var: " << array_var.get_sort() << std::endl;
#endif
  unsigned old_entry_size, new_entry_size;
  if(vars.find(index) == vars.end())
    vars.insert(
        std::pair<
        unsigned, 
        z3_expr_set>(
          index, 
          z3_expr_set({})));
  old_entry_size = vars[index].size();
  vars[index].insert(array_var);
  new_entry_size = vars[index].size();
  assert(new_entry_size - old_entry_size == 1 ||
	 new_entry_size - old_entry_size == 0);
  size += new_entry_size - old_entry_size;
}

bool axdinterpolator::ArrayVars::isEmpty() const {
  return (size == 0);
}

unsigned axdinterpolator::ArrayVars::getSize() const {
  return size;
}

bool axdinterpolator::ArrayVars::areCommonPairsAvaible() const {
  for(auto const & entry : vars){
    if(entry.second.size() >= 2)
      return true;
  }
  return false;
}

axdinterpolator::ArrayVars::Container::const_iterator axdinterpolator::ArrayVars::begin() const {
  return vars.begin();
}

axdinterpolator::ArrayVars::Container::const_iterator axdinterpolator::ArrayVars::end() const {
  return vars.end();
}

std::ostream & operator << (std::ostream & os, 
    axdinterpolator::ArrayVars const & vars){
  for(auto const & entry : vars){
    os << "Id of sort: ";
    os << entry.first << std::endl;
    for(auto const & x : entry.second)
      os << x << " ";
    os << std::endl;
  }
  return os;
}
