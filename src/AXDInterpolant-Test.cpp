#include "AXDInterpolant.h"
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

// [TODO]: parametrize array_sort and element_sort axioms
void AXDInterpolant::testOutputArrayAxiomatization(z3::solver & s){
  //z3::expr x = sig.ctx.constant("x", this->sig.array_sort);
  //z3::expr y = sig.ctx.constant("y", this->sig.array_sort);
  //z3::expr e = sig.ctx.constant("e", this->sig.element_sort);
  //z3::expr i = sig.ctx.constant("i", this->sig.int_sort);
  //z3::expr j = sig.ctx.constant("j", this->sig.int_sort);
  //z3::expr n = sig.ctx.constant("n", this->sig.int_sort);
  //// Adding axiomatization
  //s.add(forall(y, i, e, 
        //z3::implies( i >= 0, sig.rd(sig.wr(y, i, e), i) == e)));
  //s.add(forall(y, i , j, e, 
        //z3::implies(i != j, sig.rd(sig.wr(y, i, e), j) == sig.rd(y, j))));
  //s.add(forall(x, y, 
        //z3::implies(x != y, sig.rd(x, sig.diff(x, y)) != sig.rd(y, sig.diff(x, y)))));
  //s.add(forall(x, y, i, 
        //z3::implies(i > sig.diff(x, y), sig.rd(x, i) == sig.rd(y, i))));
  //s.add(forall(x, 
        //sig.diff(x, x) == 0));
  //s.add(forall(i, x, 
        //z3::implies(i < 0, sig.rd(x, i) == sig.undefined)));
  //s.add(forall(i, 
        //sig.rd(sig.empty_array, i) == sig.undefined));
  //return;
}

void AXDInterpolant::testOutputDiffLifting(z3::solver & s, StandardInput const & input){
  // Adding equations of new symbols
  for(auto const & diff_entry : input.diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned diff_iteration = 1;
    for(auto const & k_ : diff_seq)
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        //s.add(k_ == sig.diff_k(sig.ctx.int_val(diff_iteration), diff_a, diff_b));
        s.add(k_ == input.diff_map.lift_diff_k(diff_iteration, diff_a, diff_b));
        diff_iteration++;
      }
  }
}
