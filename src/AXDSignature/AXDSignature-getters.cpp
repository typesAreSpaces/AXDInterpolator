#include "AXDSignature.h"
#include "util.h"

#define EXTENDED_GET(DATA_STRUCTURE)                                           \
  if (isArraySort(sort)) {                                                     \
    auto const &name_sort = sort.to_string().substr(9);                        \
    auto const &curr_array_sort =                                              \
	ctx.uninterpreted_sort(("ArraySortArrayInt" + name_sort).c_str());     \
    return DATA_STRUCTURE[array_sort_map.at(curr_array_sort.id())];            \
  }                                                                            \
  return DATA_STRUCTURE[array_sort_map.at(sort.id())];
#define GET(DATA_STRUCTURE) return DATA_STRUCTURE[array_sort_map.at(sort.id())];

axdinterpolator::AXDSignature::TheoryName const &

axdinterpolator::AXDSignature::getTheoryName() const {
  return theory_name;
}

z3::sort
axdinterpolator::AXDSignature::getArraySortBySort(z3::sort const &sort) const {
  // m_out << "@getArraySortBySort" << std::endl;
  GET(array_sorts);
}

z3::expr
axdinterpolator::AXDSignature::getUndefinedBySort(z3::sort const &sort) const {
  // m_out << "@getUndefinedBySort" << std::endl;
  // m_out << "This is not working properly :(" << std::endl;
  // m_out << "Sort: " << sort << std::endl;
  // m_out << "Sort id: " << sort.id() << std::endl;
  // m_out << "Index from array_sort_map: " << array_sort_map.at(sort.id())
  // 	<< std::endl;
  // return undefined_es[array_sort_map.at(sort.id())];
  EXTENDED_GET(undefined_es);
}

z3::expr
axdinterpolator::AXDSignature::getEmptyArrayBySort(z3::sort const &sort) const {
  // m_out << "@getEmptyArrayBySort" << std::endl;
  // return empty_array_es[array_sort_map.at(sort.id())];
  GET(empty_array_es);
}

z3::func_decl
axdinterpolator::AXDSignature::getDiffBySort(z3::sort const &sort) const {
  // m_out << "@getDiffBySort" << std::endl;
  // return diff_es[array_sort_map.at(sort.id())];
  GET(diff_es);
}

z3::func_decl
axdinterpolator::AXDSignature::getDiff_BySort(z3::sort const &sort) const {
  // m_out << "@getDiff_BySort" << std::endl;
  // return diff_k_es[array_sort_map.at(sort.id())];
  GET(diff_k_es);
}

z3::func_decl
axdinterpolator::AXDSignature::getWrBySort(z3::sort const &sort) const {
  // m_out << "@getWrBySort" << std::endl;
  // return wr_es[array_sort_map.at(sort.id())];
  GET(wr_es);
}

z3::func_decl
axdinterpolator::AXDSignature::getRdBySort(z3::sort const &sort) const {
  // m_out << "@getRdBySort" << std::endl;
  // return rd_es[array_sort_map.at(sort.id())];
  GET(rd_es);
}

z3::func_decl
axdinterpolator::AXDSignature::getLengthBySort(z3::sort const &sort) const {
  // m_out << "@getLengthBySort" << std::endl;
  // m_out << sort << " : " << sort.id() << std::endl;
  // basicPrintMap(array_sort_map);
  // return length_es[array_sort_map.at(sort.id())];
  GET(length_es);
}
