#include "AXDSignature.h"

AXDSignature::TheoryName const & AXDSignature::getTheoryName() const {
  return theory_name;
}

z3::sort AXDSignature::getArraySortBySort(z3::sort const & sort) const {
  return array_sorts[arraySortMap.at(sort.id())];
}

z3::sort AXDSignature::getArraySortBySort(unsigned sort_id) const {
 return array_sorts[arraySortMap.at(sort_id)];
}

z3::expr AXDSignature::getUndefinedBySort(z3::sort const & sort) const {
  return undefined_es[arraySortMap.at(sort.id())];
}

z3::expr AXDSignature::getUndefinedBySort(unsigned sort_id) const {
  return undefined_es[arraySortMap.at(sort_id)];
}

z3::expr AXDSignature::getEmptyArrayBySort(z3::sort const & sort) const {
  return empty_array_es[arraySortMap.at(sort.id())];
}

z3::expr AXDSignature::getEmptyArrayBySort(unsigned sort_id) const {
  return empty_array_es[arraySortMap.at(sort_id)];
}

z3::func_decl AXDSignature::getDiffBySort(z3::sort const & sort) const {
  return diff_es[arraySortMap.at(sort.id())];
}

z3::func_decl AXDSignature::getDiffBySort(unsigned sort_id) const {
  return diff_es[arraySortMap.at(sort_id)];
}

z3::func_decl AXDSignature::getDiff_BySort(z3::sort const & sort) const {
  return diff_k_es[arraySortMap.at(sort.id())];
}

z3::func_decl AXDSignature::getDiff_BySort(unsigned sort_id) const {
  return diff_k_es[arraySortMap.at(sort_id)];
}

z3::func_decl AXDSignature::getWrBySort(z3::sort const & sort) const {
  return wr_es[arraySortMap.at(sort.id())];
}

z3::func_decl AXDSignature::getWrBySort(unsigned sort_id) const {
  return wr_es[arraySortMap.at(sort_id)];
}

z3::func_decl AXDSignature::getRdBySort(z3::sort const & sort) const {
  return rd_es[arraySortMap.at(sort.id())];
}

z3::func_decl AXDSignature::getRdBySort(unsigned sort_id) const {
  return rd_es[arraySortMap.at(sort_id)];
}
