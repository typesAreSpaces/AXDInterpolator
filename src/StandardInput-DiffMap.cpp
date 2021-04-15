#include "StandardInput.h"

StandardInput::DiffMap::DiffMap(
    Preprocessor::ArrayVars const & array_var_ids, 
    AXDSignature const & sig) : 
  m_map(),
  sig(sig)
{

  for(auto const & y : array_var_ids){
    for(auto const & _x : y.second){
      for(auto const & _y : y.second){
        if(_x.id() > _y.id())
          m_map.insert(std::make_pair(
                z3_expr_pair(_x, _y), 
                DiffMapEntry(sig, _x, _y)));
        if(_y.id() > _x.id())
          m_map.insert(std::make_pair(
                z3_expr_pair(_y, _x), 
                DiffMapEntry(sig, _y, _x)));
      }
    }

  }
}

bool StandardInput::DiffMap::Z3ExprExprComparator::operator() (
    z3_expr_pair const & a, z3_expr_pair const & b) const {
  return a.first.id() > b.first.id() 
    || (a.first.id() == b.first.id() 
        && a.second.id() > b.second.id());
}

void StandardInput::DiffMap::add(
    z3::expr const & a, 
    z3::expr const & b, 
    z3::expr const & index){
  if(a.id() > b.id()){
    add_aux(a, b, index);
    return;
  }
  if(b.id() > a.id())
    add_aux(b, a, index);
}

void StandardInput::DiffMap::add_aux(
    z3::expr const & a, 
    z3::expr const & b, 
    z3::expr const & index){
  auto table_entry = m_map.find(std::make_pair(a, b));
  if(table_entry == m_map.end()){
    throw 
      "Problem @ StandardInput::DiffMap::add_aux. "
      "Query a pair that should'nt be in the map";
  }
  table_entry->second.push(index, a, b);
}

unsigned StandardInput::DiffMap::size_of_entry(
    z3_expr_pair const & entry){
  return m_map.find(entry)->second.size();
}

z3::expr StandardInput::DiffMap::lift_diff_k(
    unsigned index,
    z3::expr const & a,
    z3::expr const & b) const {
  auto table_entry = m_map.find(std::make_pair(a, b));
  if(table_entry == m_map.end()){
    throw 
      "Problem @ StandardInput::DiffMap::add_aux. "
      "Query a pair that should'nt be in the map";
  }
  return table_entry->second.lifted_diff_k[index];
}

std::ostream & operator << (std::ostream & os, 
    StandardInput::DiffMap const & dm){

  for(auto const & x : dm.m_map){
    os 
      << x.first.first << " " << x.first.second 
      << " -> " << x.second 
      << std::endl;
  }
  return os;
}
