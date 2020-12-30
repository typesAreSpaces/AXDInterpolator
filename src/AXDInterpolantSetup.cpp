#include "AXDInterpolant.h"

void AXDInterpolant::SmtSolverSetup(
    z3::solver & solver, 
    StandardInput const & side_part){

  for(auto const & assertion : side_part.part_2)
    solver.add(assertion);

  // axiom_8
  side_part.instantiate(solver, (z3::expr &) side_part.axiom_8);

  // axiom_9
  side_part.instantiate(solver, (z3::expr &) side_part.axiom_9);

  // axiom_11_2 [WriteVector: a, b, i],
  for(auto const & _4tuple 
      : side_part.write_vector.m_vector){
    auto const & a = std::get<0>(_4tuple);
    auto const & b = std::get<1>(_4tuple);
    auto const & i = std::get<2>(_4tuple);

    z3::expr axiom_11_2 = 
      z3::implies(
          side_part.index_var != i,
          rd(a, side_part.index_var) == rd(b, side_part.index_var)
          );
    side_part.instantiate(solver, axiom_11_2);
  }

  // axiom_18 (axiom_12_2) [DiffMap: a, b, l]
  for(auto const & diff_entry 
      : side_part.diff_map.m_map){
    auto const & a = diff_entry.first.first;
    auto const & b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;

    if(diff_seq.size() == 0)
      continue;

    unsigned last_one = diff_seq.size() - 1;
    z3::expr_vector disj_equalities(ctx);
    disj_equalities.push_back(
        rd(a, side_part.index_var) 
        == rd(b, side_part.index_var));
    for(unsigned i = 0; i < last_one; i++)
      disj_equalities.push_back(
          side_part.index_var == diff_seq[i]);

    z3::expr axiom_18 = z3::implies(
        side_part.index_var > diff_seq[last_one], 
        z3::mk_or(disj_equalities));
    side_part.instantiate(solver, axiom_18);
  }
}

void AXDInterpolant::SmtSolverOutStreamSetup(
    std::ostream & out, 
    StandardInput const & side_part){
  for(auto const & assertion : side_part.part_2)
    out << assertion << std::endl;

  // axiom_8
  side_part.instantiate(out, (z3::expr &) side_part.axiom_8);

  // axiom_9
  side_part.instantiate(out, (z3::expr &) side_part.axiom_9);

  // axiom_11_2 [WriteVector: a, b, i],
  for(auto const & _4tuple 
      : side_part.write_vector.m_vector){
    auto const & a = std::get<0>(_4tuple);
    auto const & b = std::get<1>(_4tuple);
    auto const & i = std::get<2>(_4tuple);

    z3::expr axiom_11_2 = 
      z3::implies(
          side_part.index_var != i,
          rd(a, side_part.index_var) == rd(b, side_part.index_var)
          );
    side_part.instantiate(out, axiom_11_2);
  }

  // axiom_18 (axiom_12_2) [DiffMap: a, b, l]
  for(auto const & diff_entry 
      : side_part.diff_map.m_map){
    auto const & a = diff_entry.first.first;
    auto const & b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;

    if(diff_seq.size() == 0)
      continue;

    unsigned last_one = diff_seq.size() - 1;
    z3::expr_vector disj_equalities(ctx);
    disj_equalities.push_back(
        rd(a, side_part.index_var) 
        == rd(b, side_part.index_var));
    for(unsigned i = 0; i < last_one; i++)
      disj_equalities.push_back(
          side_part.index_var == diff_seq[i]);

    z3::expr axiom_18 = z3::implies(
        side_part.index_var > diff_seq[last_one], 
        z3::mk_or(disj_equalities));
    side_part.instantiate(out, axiom_18);
  }
}

void AXDInterpolant::AB_VectorsSetup(
    z3::expr_vector & part_vector, 
    StandardInput const & side_part){
  for(auto const & assertion : side_part.part_2)
    part_vector.push_back(assertion);

  // axiom_8
  side_part.instantiate(part_vector, (z3::expr &) side_part.axiom_8);

  // axiom_9
  side_part.instantiate(part_vector, (z3::expr &) side_part.axiom_9);

  // axiom_11_2 [WriteVector: a, b, i],
  for(auto const & _4tuple 
      : side_part.write_vector.m_vector){
    auto const & a = std::get<0>(_4tuple);
    auto const & b = std::get<1>(_4tuple);
    auto const & i = std::get<2>(_4tuple);

    z3::expr axiom_11_2 = 
      z3::implies(
          side_part.index_var != i,
          rd(a, side_part.index_var) == rd(b, side_part.index_var)
          );
    side_part.instantiate(part_vector, axiom_11_2);
  }

  // axiom_18 (axiom_12_2) [DiffMap: a, b, l]
  for(auto const & diff_entry 
      : side_part.diff_map.m_map){
    auto const & a = diff_entry.first.first;
    auto const & b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;

    if(diff_seq.size() == 0)
      continue;

    unsigned last_one = diff_seq.size() - 1;
    z3::expr_vector disj_equalities(ctx);
    disj_equalities.push_back(
        rd(a, side_part.index_var) 
        == rd(b, side_part.index_var));
    for(unsigned i = 0; i < last_one; i++)
      disj_equalities.push_back(
          side_part.index_var == diff_seq[i]);

    z3::expr axiom_18 = z3::implies(
        side_part.index_var > diff_seq[last_one], 
        z3::mk_or(disj_equalities));
    side_part.instantiate(part_vector, axiom_18);
  }
}
