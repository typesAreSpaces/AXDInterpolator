#include "StandardInput.h"

StandardInput::DiffMapEntry::DiffMapEntry(
    AXDSignature const & sig, 
    z3::expr const & a, 
    z3::expr const & b,
    unsigned sort_index
    ) : 
  z3::expr_vector(sig.ctx), 
  sort_index(sort_index),
  lifted_b(sig.ctx), 
  lifted_diff_k(sig.ctx),
  sig(sig)
{
  lifted_b.push_back(b);
  lifted_diff_k.push_back(
      sig.getDiffBySort(sort_index)(a, b));
}

void StandardInput::DiffMapEntry::push(
    z3::expr const & index, 
    z3::expr const & a,
    z3::expr const & b
    ){
  push_back(index);
  unsigned last_index = this->size();
  auto const & last_b = lifted_b[last_index - 1];
  auto const & last_diff = lifted_diff_k[last_index - 1];
  // [TODO] use appropriate 
  // sig.wr and sig.rd
  auto const & new_b = sig.wr(last_b, last_diff, sig.rd(a, last_diff));
  lifted_b.push_back(new_b);
  // [TODO] use appropriate 
  // sig.diff
  lifted_diff_k.push_back(sig.diff(a, new_b));
}
