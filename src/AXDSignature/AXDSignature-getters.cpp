#include "AXDSignature.h"

axdinterpolator::AXDSignature::TheoryName const & axdinterpolator::AXDSignature::getTheoryName() const {
  return theory_name;
}

z3::sort axdinterpolator::AXDSignature::getArraySortBySort(z3::sort const & sort) const {
  return array_sorts[array_sort_map.at(sort.id())];
}

z3::sort axdinterpolator::AXDSignature::getArraySortBySort(unsigned sort_id) const {
 return array_sorts[array_sort_map.at(sort_id)];
}

z3::expr axdinterpolator::AXDSignature::getUndefinedBySort(z3::sort const & sort) const {
  return undefined_es[array_sort_map.at(sort.id())];
}

z3::expr axdinterpolator::AXDSignature::getUndefinedBySort(unsigned sort_id) const {
  return undefined_es[array_sort_map.at(sort_id)];
}

z3::expr axdinterpolator::AXDSignature::getEmptyArrayBySort(z3::sort const & sort) const {
  return empty_array_es[array_sort_map.at(sort.id())];
}

z3::expr axdinterpolator::AXDSignature::getEmptyArrayBySort(unsigned sort_id) const {
  return empty_array_es[array_sort_map.at(sort_id)];
}

z3::func_decl axdinterpolator::AXDSignature::getDiffBySort(z3::sort const & sort) const {
  return diff_es[array_sort_map.at(sort.id())];
}

z3::func_decl axdinterpolator::AXDSignature::getDiffBySort(unsigned sort_id) const {
  return diff_es[array_sort_map.at(sort_id)];
}

z3::func_decl axdinterpolator::AXDSignature::getDiff_BySort(z3::sort const & sort) const {
  return diff_k_es[array_sort_map.at(sort.id())];
}

z3::func_decl axdinterpolator::AXDSignature::getDiff_BySort(unsigned sort_id) const {
  return diff_k_es[array_sort_map.at(sort_id)];
}

z3::func_decl axdinterpolator::AXDSignature::getWrBySort(z3::sort const & sort) const {
  return wr_es[array_sort_map.at(sort.id())];
}

z3::func_decl axdinterpolator::AXDSignature::getWrBySort(unsigned sort_id) const {
  return wr_es[array_sort_map.at(sort_id)];
}

z3::func_decl axdinterpolator::AXDSignature::getRdBySort(z3::sort const & sort) const {
  return rd_es[array_sort_map.at(sort.id())];
}

z3::func_decl axdinterpolator::AXDSignature::getRdBySort(unsigned sort_id) const {
  return rd_es[array_sort_map.at(sort_id)];
}

z3::func_decl axdinterpolator::AXDSignature::getLengthBySort(z3::sort const & sort) const {
  return length_es[array_sort_map.at(sort.id())];
}

z3::func_decl axdinterpolator::AXDSignature::getLengthBySort(unsigned sort_id) const {
  return length_es[array_sort_map.at(sort_id)];
}
