#include "AXDInterpolant.h"

// Precondition: part_a_vector and part_b_vector should be updated using
// setupPartA_B_Vectors
void AXDInterpolant::testOutput(
    z3::expr const & interpolant, 
    z3::expr_vector & part_a_vector, z3::expr_vector & part_b_vector){
  system(("mkdir -p " + OUTPUT_DIR).c_str());
  std::ofstream test1_file(
      OUTPUT_DIR + "/" + m_file_name + "_test1.smt2");
  std::ofstream test2_file(
      OUTPUT_DIR + "/" + m_file_name + "_test2.smt2");

  z3::solver test1(ctx);
#if _TEST_ORIGINAL_INPUT_
  test1.add(not(z3::implies(z3::mk_and(input_part_a), interpolant)));
  testOutputArrayAxiomatization(test1);
  testOutputDiffLifting(test1, part_a);
  testOutputDiffLifting(test1, part_b);
#else
  test1.add(not(z3::implies(z3::mk_and(part_a_vector), interpolant)));
#endif
  test1_file << test1.to_smt2();

  z3::solver test2(ctx);
#if _TEST_ORIGINAL_INPUT_
  test2.add(z3::mk_and(input_part_b) && interpolant);
  testOutputArrayAxiomatization(test2);
  testOutputDiffLifting(test2, part_a);
  testOutputDiffLifting(test2, part_b);
#else
  test2.add(mk_and(part_b_vector) && interpolant);
#endif
  test2_file << test2.to_smt2();
}

void AXDInterpolant::testOutputArrayAxiomatization(z3::solver & s){
  z3::expr x = ctx.constant("x", this->array_sort);
  z3::expr y = ctx.constant("y", this->array_sort);
  z3::expr e = ctx.constant("e", this->element_sort);
  z3::expr i = ctx.constant("i", this->int_sort);
  z3::expr j = ctx.constant("j", this->int_sort);
  // Adding axiomatization
  s.add(forall(y, i, e, rd(wr(y, i, e), i) == e));
  s.add(forall(y, i , j, e, z3::implies(i != j, rd(wr(y, i, e), j) == rd(y, j))));
  s.add(forall(x, y, z3::implies(x != y, rd(x, diff(x, y)) != rd(y, diff(x, y)))));
  s.add(forall(x, y, i, z3::implies(i > diff(x, y), rd(x, i) == rd(y, i))));
  s.add(forall(x, diff(x, x) == 0));
  return ;
}

void AXDInterpolant::testOutputDiffLifting(z3::solver & s, StandardInput const & input){
  // Adding equations of new symbols
  for(auto const & diff_entry : input.diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned diff_iteration = 1;
    for(auto const & k_ : diff_seq)
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0)
        s.add(k_ == diff_k(ctx.int_val(diff_iteration++), diff_a, diff_b));
  }
}


