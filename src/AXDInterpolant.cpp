#include "AXDInterpolant.h"
#include "z3++.h"

AXDInterpolant::AXDInterpolant(
    AXDSignature & sig, 
    z3::expr const & _input_part_a,
    z3::expr const & _input_part_b,
    char const * file_name,
    unsigned allowed_attempts) : 
  Preprocessor(sig, _input_part_a, _input_part_b),
  part_a(
      sig,
      input_part_a,
      part_a_index_vars, 
      part_a_array_vars),
  part_b(
      sig,
      input_part_b,
      part_b_index_vars, 
      part_b_array_vars),
  m_file_name(std::string(file_name)),
  num_attempts(0), remaining_fuel(allowed_attempts),
  is_interpolant_computed(false), 
  is_unsat(false), 
  is_valid_result(false),
  state_output(undefined),
  current_interpolant(sig.ctx.bool_val(true)),
  solver(sig.ctx, 
      sig.is_QF_TO() || sig.is_QF_IDL() ? "QF_UFIDL" : "QF_UFLIA"
      )
{
#if _DEBUG_AXD_CONSTRUCTOR_
  std::cout << "After Processing" << std::endl;
  std::cout << "Input Part A" << std::endl;
  std::cout << input_part_a << std::endl;
  std::cout << "Input Part B" << std::endl;
  std::cout << input_part_b << std::endl;

  std::cout << "Before StandardInput" << std::endl;
  std::cout << "Input Part A" << std::endl;
  std::cout << part_a << std::endl;
  std::cout << "Input Part B" << std::endl;
  std::cout << part_b << std::endl;
#endif

  m_out << "Solving file " << m_file_name << std::endl;
  m_out << "Input formula" << std::endl;
  m_out << "Part A" << std::endl;
  m_out << part_a << std::endl;
  m_out << "Part B" << std::endl;
  m_out << part_b << std::endl;

  m_file_name = m_file_name
    .substr(0, m_file_name.find_last_of("."))
    .substr(m_file_name.find_last_of("\\/") + 1);

  loop();
}

void AXDInterpolant::loop(){

  if(common_array_vars.getSize() < 2){
    SmtSolverSetup(solver, part_a);
    SmtSolverSetup(solver, part_b);

    if(solver.check() == z3::unsat){
#if _DEBUG_AXD_LOOP_ 
      m_out << "Iteration #0 no common array symbols" << std::endl;
      m_out << "Current A-part part 2: " << std::endl;
      SmtSolverOutStreamSetup(m_out, part_a);
      m_out << "Current B-part part 2: " << std::endl;
      SmtSolverOutStreamSetup(m_out, part_b);
#endif
      is_unsat = true;
#if _DEBUG_AXD_LOOP_
      m_out << "Unsat after 0 iterations" << std::endl;
#endif
    }
    return;
  }

  CircularPairIterator search_common_pair(common_array_vars);

  while(num_attempts++ < remaining_fuel){
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
        << num_attempts << std::endl;
      m_out 
        << "Current A-part part 2: " << std::endl;
      SmtSolverOutStreamSetup(m_out, part_a);
      m_out 
        << "Current B-part part 2: " << std::endl;
      SmtSolverOutStreamSetup(m_out, part_b);
#endif

      is_unsat = true;
#if _DEBUG_AXD_LOOP_
      m_out 
        << "Unsat after " 
        << num_attempts 
        << " iterations" 
        << std::endl;
#endif
      return;
    }

    solver.pop();
#if _DEBUG_AXD_LOOP_ 
    m_out 
      << "Iteration #" 
      << num_attempts  << std::endl;
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

  z3::expr_vector _interpolant(sig.ctx);

  if(sig.is_QF_TO())
    for(auto const & x : interpolant)
      _interpolant.push_back(x.qf_to_simplify());
  else
    for(auto const & x : interpolant)
      _interpolant.push_back(x);

  z3::expr_vector from(sig.ctx);
  z3::expr_vector to(sig.ctx);

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
    auto const & curr_diff_k = sig.getDiff_BySort(diff_a.get_sort());
    unsigned diff_iteration = 1;
    for(auto const & k_ : diff_seq){
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        from.push_back(k_);
        to.push_back(curr_diff_k(
              sig.ctx.int_val(diff_iteration++), 
              diff_a, diff_b));
      }
    }
  }
}

std::ostream & operator << (std::ostream & os, 
    AXDInterpolant const & axd){

  if(!axd.remaining_fuel)
    return os 
      << "Unknown: Input formula might be "
      "satisfiable or unsatisfiable.";
  if(!axd.is_unsat)
    return os 
      << "Input formula is satisfiable.";
  if(!axd.is_valid_result)
    return os 
      << "Input formula is unsatisfiable.\n"
      "Solver used wasn't able to compute "
      "an interpolant in the specified theory";
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
#if _TEST_OUTPUT_ORIGINAL_THY_
          << "State Output: valid in the original theory" 
#else
          << "State Output: valid in the reduced theory" 
#endif
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
