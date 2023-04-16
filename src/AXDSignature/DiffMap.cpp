#include "AXDSignature.h"

// Creates for each
// pairs of ids in the domain of array_var_ids
// an empty DiffMapEntry
axdinterpolator::DiffMap::DiffMap(
    axdinterpolator::ArrayVars const &array_var_ids, AXDSignature const &sig)
    : m_map(), sig(sig) {

  for (auto const &entry : array_var_ids) {
    for (auto const &array_var_1 : entry.second) {
      for (auto const &array_var_2 : entry.second) {
	if (array_var_1.id() > array_var_2.id())
	  m_map.insert(
	      std::make_pair(z3_expr_pair(array_var_1, array_var_2),
			     DiffMapEntry(sig, array_var_1, array_var_2)));
	if (array_var_2.id() > array_var_1.id())
	  m_map.insert(
	      std::make_pair(z3_expr_pair(array_var_2, array_var_1),
			     DiffMapEntry(sig, array_var_2, array_var_1)));
      }
    }
  }
}

bool axdinterpolator::DiffMap::Z3ExprExprComparator::operator()(
    z3_expr_pair const &a, z3_expr_pair const &b) const {
  return a.first.id() > b.first.id() ||
	 (a.first.id() == b.first.id() && a.second.id() > b.second.id());
}

void axdinterpolator::DiffMap::add(z3::expr const &a, z3::expr const &b,
				   z3::expr const &index) {
  if (a.id() > b.id()) {
    add_aux(a, b, index);
    return;
  }
  if (b.id() > a.id())
    add_aux(b, a, index);
}

void axdinterpolator::DiffMap::add_aux(z3::expr const &a, z3::expr const &b,
				       z3::expr const &index) {
  auto table_entry = m_map.find(std::make_pair(a, b));
  if (table_entry == m_map.end()) {
    throw "Problem @ axdinterpolator::DiffMap::add_aux. "
	  "Query a pair that should'nt be in the map";
  }
  table_entry->second.push(index, a, b);
}

unsigned axdinterpolator::DiffMap::size_of_entry(z3_expr_pair const &entry) {
  return m_map.find(entry)->second.size();
}

z3::expr axdinterpolator::DiffMap::lift_diff_k(unsigned index,
					       z3::expr const &a,
					       z3::expr const &b) const {
  auto table_entry = m_map.find(std::make_pair(a, b));
  if (table_entry == m_map.end()) {
    throw "Problem @ axdinterpolator::DiffMap::add_aux. "
	  "Query a pair that should'nt be in the map";
  }
  return table_entry->second.lifted_diff_k[index];
}

namespace axdinterpolator {

std::ostream &operator<<(std::ostream &os, axdinterpolator::DiffMap const &dm) {

  for (auto const &x : dm.m_map) {
    os << x.first.first << " " << x.first.second << " -> " << x.second
       << std::endl;
  }
  return os;
}

} // namespace axdinterpolator
