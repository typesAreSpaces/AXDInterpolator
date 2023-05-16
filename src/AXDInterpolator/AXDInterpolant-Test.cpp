#include "_AXDInterpolant.h"
#include "z3++.h"

// Precondition: 
// part_a_vector and part_b_vector 
// should have been updated using
// setupPartA_B_Vectors
bool AXDInterpolant::testOutput(
    z3::expr_vector const & interpolant, 
    z3::expr_vector & part_a_vector, 
    z3::expr_vector & part_b_vector){

  z3::solver test1(sig.ctx), test2(sig.ctx);

#if _TEST_OUTPUT_ORIGINAL_THY_
  test1.add(
      not(z3::implies(
          z3::mk_and(input_part_a), 
          z3::mk_and(interpolant))));
  testOutputArrayAxiomatization(test1);
  testOutputDiffLifting(test1, part_a);
  testOutputDiffLifting(test1, part_b);
#else
  test1.add(
      not(z3::implies(
          z3::mk_and(part_a_vector), 
          z3::mk_and(interpolant))));
#endif

#if _TEST_OUTPUT_ORIGINAL_THY_
  test2.add(
      z3::mk_and(input_part_b) 
      && z3::mk_and(interpolant));
  testOutputArrayAxiomatization(test2);
  testOutputDiffLifting(test2, part_a);
  testOutputDiffLifting(test2, part_b);
#else
  test2.add(
      mk_and(part_b_vector) 
      && z3::mk_and(interpolant));
#endif

  if(test1.check() == z3::unsat){
    //std::cout << "Done with A-part testing - PASSED" << std::endl;
    return test2.check() == z3::unsat;
  }

  return false;
}

void AXDInterpolant::testOutputArrayAxiomatization(z3::solver & s){

  z3::expr i = sig.ctx.constant("i", sig.int_sort);
  z3::expr j = sig.ctx.constant("j", sig.int_sort);
  z3::expr n = sig.ctx.constant("n", sig.int_sort);

  for(unsigned m = 0; m < sig.element_sorts.size(); m++){

    auto const & curr_element_sort = sig.element_sorts[m];
    auto const & curr_array_sort = sig.array_sorts[m];
    auto const & curr_rd = sig.rd_es[m];
    auto const & curr_wr = sig.wr_es[m];
    auto const & curr_diff = sig.diff_es[m];
    auto const & curr_undefined = sig.undefined_es[m];
    auto const & curr_empty_array = sig.empty_array_es[m];

    z3::expr x = sig.ctx.constant("x", curr_array_sort);
    z3::expr y = sig.ctx.constant("y", curr_array_sort);
    z3::expr e = sig.ctx.constant("e", curr_element_sort);

    s.add(forall(y, i, e, 
          z3::implies( i >= 0, curr_rd(curr_wr(y, i, e), i) == e)));
    s.add(forall(y, i , j, e, 
          z3::implies(i != j, curr_rd(curr_wr(y, i, e), j) == curr_rd(y, j))));
    s.add(forall(x, y, 
          z3::implies(x != y, curr_rd(x, curr_diff(x, y)) != curr_rd(y, curr_diff(x, y)))));
    s.add(forall(x, y, i, 
          z3::implies(i > curr_diff(x, y), curr_rd(x, i) == curr_rd(y, i))));
    s.add(forall(x, 
          curr_diff(x, x) == 0));
    s.add(forall(i, x, 
          z3::implies(i < 0, curr_rd(x, i) == curr_undefined)));
    s.add(forall(i, 
          curr_rd(curr_empty_array, i) == curr_undefined));
  }
  return;
}

void AXDInterpolant::testOutputDiffLifting(
    z3::solver & s, StandardInput const & input){
  // Adding equations of new symbols
  for(auto const & diff_entry : input.diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned diff_iteration = 0;
    for(auto const & k_ : diff_seq){
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        //s.add(
        //k_ == sig.diff_k(sig.ctx.int_val(diff_iteration), diff_a, diff_b));
        s.add(
            k_ 
            == input.diff_map.lift_diff_k(diff_iteration, diff_a, diff_b));
        diff_iteration++;
      }
    }
  }
}
