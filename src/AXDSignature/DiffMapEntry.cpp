#include "AXDSignature.h"

axdinterpolator::DiffMapEntry::DiffMapEntry(
    AXDSignature const & sig, 
    z3::expr const & a, 
    z3::expr const & b) : 
  z3::expr_vector(sig.ctx), 
  lifted_b(sig.ctx), 
  lifted_diff_k(sig.ctx),
  sig(sig)
{
  lifted_b.push_back(b);
  lifted_diff_k.push_back(
      sig.getDiffBySort(a.get_sort())(a, b));
}

void axdinterpolator::DiffMapEntry::push(
    z3::expr const & index, 
    z3::expr const & a,
    z3::expr const & b){
  push_back(index);
  unsigned last_index = this->size();
  auto const & last_b = lifted_b[last_index - 1];
  auto const & last_diff = lifted_diff_k[last_index - 1];
  auto const & curr_wr = sig.getWrBySort(a.get_sort());
  auto const & curr_rd = sig.getRdBySort(a.get_sort());
  auto const & curr_diff = sig.getDiffBySort(a.get_sort());
  auto const & new_b = curr_wr(last_b, last_diff, curr_rd(a, last_diff));
  lifted_b.push_back(new_b);
  lifted_diff_k.push_back(curr_diff(a, new_b));
}
