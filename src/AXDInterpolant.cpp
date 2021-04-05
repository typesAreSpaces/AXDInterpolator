#include "AXDInterpolant.h"
#include "z3++.h"

AXDInterpolant::AXDInterpolant(
    z3::context & ctx, 
    z3::expr const & assertions,
    char const * file_name,
    char const * _theory_name,
    unsigned allowed_attempts) : 
  Preprocessor(ctx, assertions),
  solver(ctx), 
  part_a(
      input_part_a,
      part_a_index_vars, 
      part_a_array_vars, 
      _theory_name,
      fresh_index
      ),
  part_b(
      input_part_b,
      part_b_index_vars, 
      part_b_array_vars,
      _theory_name,
      part_a.get_fresh_index()),
  m_file_name(std::string(file_name)),
  num_attempts(allowed_attempts),
  is_interpolant_computed(false), is_unsat(false),
  current_interpolant(ctx.bool_val(true)),
  theory_name(_theory_name),
  state_output(undefined)
{
  m_out
    << "Solving file " << m_file_name 
    << std::endl;
  m_file_name = m_file_name
    .substr(0, m_file_name.find_last_of("."))
    .substr(m_file_name.find_last_of("\\/") + 1);

  loop();
}

void AXDInterpolant::loop(){
#if _DEBUG_AXD_LOOP_
  unsigned const constant_allowed_attempts = num_attempts;
#endif
  CircularPairIterator search_common_pair(common_array_vars);

  while(--num_attempts){
    solver.push();
    // The following uses a z3::solver 
    // to check if part_a \land part_b
    // is unsat
    SmtSolverSetup(solver, part_a);
    SmtSolverSetup(solver, part_b);

    if(solver.check() == z3::unsat){
#if _DEBUG_AXD_LOOP_ 
      m_out 
        << "Iteration #" 
        << (constant_allowed_attempts - num_attempts) << std::endl;
      m_out 
        << "Current A-part part 2: " << std::endl;
      SmtSolverOutStreamSetup(m_out, part_a);
      m_out 
        << "Current B-part part 2: " << std::endl;
      SmtSolverOutStreamSetup(m_out, part_b);
#endif

      is_unsat = true;
#if _DEBUG_AXD_LOOP_
      m_out << "Unsat after " 
        << constant_allowed_attempts - num_attempts 
        << " iterations" << std::endl;
#endif
      return;
    }

    solver.pop();
#if _DEBUG_AXD_LOOP_ 
    m_out 
      << "Iteration #" 
      << (constant_allowed_attempts - num_attempts) << std::endl;
    m_out 
      << "Current A-part part 2: " << std::endl;
    SmtSolverOutStreamSetup(m_out, part_a);
    m_out 
      << "Current B-part part 2: " << std::endl;
    SmtSolverOutStreamSetup(m_out, part_b);
#endif
    // Find pair of common array variables
    auto const & common_pair = *search_common_pair;

    unsigned part_a_dim = part_a.diff_map.size_of_entry(common_pair),
             part_b_dim = part_b.diff_map.size_of_entry(common_pair),
             min_dim = std::min(part_a_dim, part_b_dim);
    auto const & _new_index = fresh_index_constant();
    part_a.updateSaturation(common_pair, _new_index, min_dim);
    part_b.updateSaturation(common_pair, _new_index, min_dim);

    search_common_pair.next();
  }
}

z3::expr AXDInterpolant::liftInterpolant(
    z3::expr_vector const & interpolant){

  z3::expr_vector _interpolant(ctx);

  if(strcmp(theory_name, "QF_TO") == 0)
    for(auto const & x : interpolant)
      _interpolant.push_back(x.qf_to_simplify());
  else
    for(auto const & x : interpolant)
      _interpolant.push_back(x);

  z3::expr_vector from(ctx);
  z3::expr_vector to(ctx);

  liftInterpolantDiffSubs(from, to, part_a);
  liftInterpolantDiffSubs(from, to, part_b);

  return z3::mk_and(_interpolant).substitute(from, to);
}

void AXDInterpolant::liftInterpolantDiffSubs(
    z3::expr_vector & from,
    z3::expr_vector & to,
    StandardInput const & input
    ){
  for(auto const & diff_entry : input.diff_map.m_map){
    auto const & diff_a   = diff_entry.first.first;
    auto const & diff_b   = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned diff_iteration = 1;
    for(auto const & k_ : diff_seq){
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        from.push_back(k_);
        to.push_back(diff_k(
              ctx.int_val(diff_iteration++), 
              diff_a, diff_b));
      }
    }
  }
}

std::ostream & operator << (std::ostream & os, 
    AXDInterpolant const & axd){

  if(!axd.num_attempts)
    return os 
      << "Unknown: Input formula might be "
      "satisfiable or unsatisfiable.";
  if(!axd.is_unsat)
    return os 
      << "Input formula is satisfiable.";
  if(axd.is_interpolant_computed){
    os 
      << "Input formula is unsatisfiable." 
      << std::endl;
    switch(axd.state_output){
      case AXDInterpolant::undefined:
        os
          << "State Output: not tested" 
          << std::endl;
        break;
      case AXDInterpolant::fine:
        os 
          << "State Output: valid" 
          << std::endl;
        break;
      case AXDInterpolant::notfine:
        os 
          << "State Output: not valid" 
          << std::endl;
        break;
    }
    return os 
#if _INCLUDE_OUTPUT_
      << "Interpolant:" << std::endl
      << axd.current_interpolant
#endif
      ;
  }
  else
    return os 
      << "Interpolant hasn't been computed.\n"
      "Use .z3OutputFile or .mathsatOutputFile\n"
      "or .directComputation on a AXDInterpolant\n" 
      "object to obtain an interpolant.";
}
