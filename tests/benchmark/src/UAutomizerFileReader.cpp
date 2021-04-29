#include "UAutomizerFileReader.h"
#include <cassert>
#include <regex>
#include <z3++.h>

#define TEMP_FILE_SETUP\
  std::string temp_file =\
  "temp_" + name_solver + "_" + current_file;\
  std::ofstream smt_file (temp_file.c_str());\
  \
  for(auto const & x : stack_of_frames)\
  smt_file << x << std::endl;\
  smt_file << current_frame << std::endl;\
  smt_file.close();\

#define BENCHMARK_COMMAND(WRITER, EXEC_COMMAND, LOG_COMMAND)\
  z3::context ctx;\
  z3::solver input_parser(ctx, "QF_AUFLIA");\
  input_parser.from_file(temp_file.c_str());\
  \
  for(auto const & x : input_parser.assertions()){\
    if(hasQuantifierAndNonLinear(x)){\
      system(("rm -rf " + temp_file).c_str());\
      return;\
    }\
  }\
  \
  if(input_parser.check() == z3::unsat){\
    std::string file_for_implementation =\
    "axdinterpolator_" +  name_solver + "_" + current_file;\
    std::ofstream axdinterpolator_file(file_for_implementation.c_str());\
    \
    z3::expr_vector curr_assertions = input_parser.assertions();\
    z3::expr_vector part_a(ctx), part_b(ctx);\
    \
    auto const & to_cnf_tactic = \
    z3::tactic(ctx, "tseitin-cnf");\
    \
    z3::goal goal_assertions(ctx);\
    goal_assertions.add(z3::mk_and(curr_assertions));\
    auto const & cnf_assertions = to_cnf_tactic(goal_assertions);\
    \
    assert(cnf_assertions.size() == 1);\
    z3::expr const & curr_conjunction = cnf_assertions[0].as_expr();\
    \
    unsigned total_size_cnf = curr_conjunction.num_args();\
    unsigned half_size_cnf = total_size_cnf/2;\
    if(curr_conjunction.decl().decl_kind() != Z3_OP_AND\
        || half_size_cnf == 0\
        || (total_size_cnf - half_size_cnf) == 0 \
        ){\
      axdinterpolator_file.close();\
      system(("rm -rf " + file_for_implementation).c_str());\
      system(("rm -rf " + temp_file).c_str());\
      return;\
    }\
    \
    part_a.push_back(ctx.bool_val(true));\
    part_b.push_back(ctx.bool_val(true));\
    for(unsigned i = 0; i < half_size_cnf; i++)\
    part_a.push_back(curr_conjunction.arg(i));\
    for(unsigned i = half_size_cnf; i < total_size_cnf; i++)\
    part_b.push_back(curr_conjunction.arg(i));\
    \
    WRITER;\
    \
    char exec_command[1000];\
    EXEC_COMMAND;\
    int ret = system(exec_command);\
    char log_command[1000];\
    LOG_COMMAND;\
    system(log_command);\
    \
    system(("rm -rf " + file_for_implementation).c_str());\
  }


UAutomizerFileReader::UAutomizerFileReader(
    SMT_SOLVER smt_solver, 
    unsigned num_samples, 
    char * const file, 
    bool test_our_implementation) : 
  line(""),
  current_frame(""),
  current_file(""),
  nesting_level(0),
  max_nesting_level(0),
  stack_of_frames({}),
  curr_solver(smt_solver),
  name_solver(
      smt_solver == Z3 ? "Z3" 
      : smt_solver == MATHSAT ? "MATHSAT" 
      : "SMTINTERPOL"),
  num_samples(num_samples),
  file_statistics(file),
  test_our_implementation(test_our_implementation)
{
}

bool UAutomizerFileReader::hasQuantifierAndNonLinear(z3::expr const & e) const {
  if(e.is_quantifier())
    return true;
  if(e.is_var())
    return true;
  if(e.is_app()){
    switch(e.decl().decl_kind()){
      case Z3_OP_REM:
      case Z3_OP_MOD:
      case Z3_OP_DIV:
      case Z3_OP_IDIV:
        return true;
      case Z3_OP_MUL:
        {
          unsigned num_numerals = 0, curr_num_args = e.num_args();
          for(unsigned i = 0; i < curr_num_args; i++)
            if(e.is_numeral())
              num_numerals++;

          if(curr_num_args - num_numerals >= 2)
            return true;
          return false;
        }
      default:
        for(unsigned i = 0; i < e.num_args(); i++)
          if (hasQuantifierAndNonLinear(e.arg(i)))
            return true;
        break;
    }
  }
  return false;
}

std::string UAutomizerFileReader::nameAssertionsZ3(std::string const & s) const {
  std::stringstream decl_strm(s);
  std::string curr_line, collected_assertion = "", ret = "";
  bool matching_assert = false, matching_part_a = true;
  unsigned num_balanced_paren = 0;

  while(std::getline(decl_strm, curr_line)){

    if(curr_line.find("assert") != std::string::npos)
      matching_assert = true;

    if(matching_assert){
      for(auto const & c : curr_line){
        if(c == '(')
          num_balanced_paren++;
        if(c == ')')
          num_balanced_paren--;
      }

      collected_assertion += curr_line;
      if(num_balanced_paren == 0){
        matching_assert = false;
        collected_assertion = collected_assertion.substr(8);
        collected_assertion.pop_back();
        if(matching_part_a){
          collected_assertion = "(assert (! " + collected_assertion +" :named part_a))";
          matching_part_a = false;
        }
        else
          collected_assertion = "(assert (! " + collected_assertion +" :named part_b))";

        ret += collected_assertion + "\n";
        collected_assertion = "";
      }
    }
    else
      ret += curr_line + "\n";
  }
  return ret;
}

std::string UAutomizerFileReader::nameAssertionsMathsat(std::string const & s) const {
  std::stringstream decl_strm(s);
  std::string curr_line, collected_assertion = "", ret = "";
  bool matching_assert = false, matching_part_a = true;
  unsigned num_balanced_paren = 0;

  while(std::getline(decl_strm, curr_line)){

    if(curr_line.find("assert") != std::string::npos)
      matching_assert = true;

    if(matching_assert){
      for(auto const & c : curr_line){
        if(c == '(')
          num_balanced_paren++;
        if(c == ')')
          num_balanced_paren--;
      }

      collected_assertion += curr_line;
      if(num_balanced_paren == 0){
        matching_assert = false;
        collected_assertion = collected_assertion.substr(8);
        collected_assertion.pop_back();
        if(matching_part_a){
          collected_assertion = "(assert (! " + collected_assertion +" :interpolation-group part_a))";
          matching_part_a = false;
        }
        else
          collected_assertion = "(assert (! " + collected_assertion +" :interpolation-group part_b))";

        ret += collected_assertion + "\n";
        collected_assertion = "";
      }
    }
    else
      ret += curr_line + "\n";
  }
  return ret;
}

bool UAutomizerFileReader::isPushCmd() const {
  return line.find("push") != std::string::npos;
}

bool UAutomizerFileReader::isPopCmd() const {
  return line.find("pop") != std::string::npos;
}

bool UAutomizerFileReader::isEchoCmd() const {
  return line.find("echo") != std::string::npos;
}

void UAutomizerFileReader::testAXDInterpolator() const {

  TEMP_FILE_SETUP;
  BENCHMARK_COMMAND(
      z3::solver tseitin_solver(ctx, "QF_AUFLIA");
      tseitin_solver.add(z3::mk_and(part_a));
      tseitin_solver.add(z3::mk_and(part_b));

      axdinterpolator_file << tseitin_solver.to_smt2();
      axdinterpolator_file.close();,

      sprintf(exec_command,
        "./../../bin/axd_interpolator QF_LIA %s %u 1000;",
        file_for_implementation.c_str(), curr_solver);,
      sprintf(log_command, 
        "echo File: \"%s\" Solver Code: \"%u\" Exit Code: %d >> \"%s\"",
        file_for_implementation.c_str(), curr_solver, ret, file_statistics);
      );
  system(("rm -rf " + temp_file).c_str());
}

void UAutomizerFileReader::testOtherSolvers() const {

  switch(curr_solver){
    case Z3:
      {
        TEMP_FILE_SETUP;
        BENCHMARK_COMMAND(
            // WRITER
            z3::solver tseitin_solver(ctx, "QF_AUFLIA");
            tseitin_solver.add(z3::mk_and(part_a));
            tseitin_solver.add(z3::mk_and(part_b));
            axdinterpolator_file 
            << "(set-option :produce-interpolants true)" 
            << std::endl;
            axdinterpolator_file 
            << "(set-logic QF_AUFLIA)" << std::endl;
            axdinterpolator_file << nameAssertionsZ3(tseitin_solver.to_smt2());
            axdinterpolator_file << "(get-interpolant part_a part_b)" << std::endl;
            axdinterpolator_file.close();,
            // EXEC_COMMAND
            std::string temp_file_name = "z3_inter_temp_" + current_file;
            sprintf(exec_command, "./../../bin/z3 %s > %s;",
              file_for_implementation.c_str(), temp_file_name.c_str());,
            // LOG_COMMAND
            std::ifstream result(temp_file_name.c_str());
            std::string line("");
            std::string interpolant_from_file("");
            // We consume two lines because
            // z3 outputs "check-sat" followed
            // a line containing "(interpolants", followed
            // by the interpolant
            std::getline(result, line);

            if(line == "unsat"){
              std::getline(result, line);

              interpolant_from_file += tseitin_solver.to_smt2_decls_only();
              interpolant_from_file += "(assert (and true\n";
              while(std::getline(result, line))
                interpolant_from_file += line + "\n";
              // Only one parenthesis is needed to close
              // the above since the content of (interpolant *)
              // includes an additional parenthesis
              interpolant_from_file += ")\n";
              interpolant_from_file += "(check-sat)\n";
              system(("rm -rf " + temp_file_name).c_str());

              z3::solver z3_interpolant_parser(ctx, "QF_AUFLIA");
              z3_interpolant_parser.from_string(interpolant_from_file.c_str());

              auto const & interpolant_result = z3_interpolant_parser.assertions();
              std::cout << interpolant_result << std::endl;

              sprintf(log_command, 
                  "echo File: \"%s\" Solver Code: \"%u\" Exit Code: %d >> \"%s\"",
                  file_for_implementation.c_str(), 3, ret, file_statistics);
            }
            else{
              system(("rm -rf " + temp_file_name).c_str());
              sprintf(log_command, 
                  "echo File: \"%s\" Solver Code: \"%u\" Exit Code: %d >> \"%s\"",
                  file_for_implementation.c_str(), 3, 1, file_statistics);
            }

            );

            system(("rm -rf " + temp_file).c_str());
      }
      return;
    case MATHSAT:
      {
        TEMP_FILE_SETUP;
        BENCHMARK_COMMAND(
            // WRITER
            z3::solver tseitin_solver(ctx, "QF_AUFLIA");
            tseitin_solver.add(z3::mk_and(part_a));
            tseitin_solver.add(z3::mk_and(part_b));
            axdinterpolator_file 
            << "(set-option :produce-interpolants true)" 
            << std::endl;
            //axdinterpolator_file 
            //<< "(set-logic QF_AUFLIA)" << std::endl;
            axdinterpolator_file << nameAssertionsMathsat(tseitin_solver.to_smt2());
            axdinterpolator_file << "(get-interpolant (part_a))" << std::endl;
            axdinterpolator_file.close();,
            // EXEC_COMMAND
            std::string temp_file_name = "mathsat_inter_temp_" + current_file;
            sprintf(exec_command, "./../../bin/mathsat %s > %s;",
              file_for_implementation.c_str(), temp_file_name.c_str());,
            // LOG_COMMAND
            std::ifstream result(temp_file_name.c_str());
            std::string line("");
            std::string interpolant_from_file("");
            // We consume two lines because
            // z3 outputs "check-sat" followed
            // a line containing "(interpolants", followed
            // by the interpolant
            std::getline(result, line);

            if(line == "unsat"){

              interpolant_from_file += tseitin_solver.to_smt2_decls_only();
              interpolant_from_file += "(assert (and true\n";
              std::string _interpolant_result = "";
              while(std::getline(result, line))
                _interpolant_result += line + "\n";

              if(_interpolant_result.find("build ie-local interpolant") != std::string::npos){
                system(("rm -rf " + temp_file_name).c_str());
                sprintf(log_command, 
                    "echo File: \"%s\" Solver Code: \"%u\" Exit Code: %d >> \"%s\"",
                    file_for_implementation.c_str(), 4, 1, file_statistics);
              }
              else {
                interpolant_from_file += _interpolant_result;
                // Only one parenthesis is needed to close
                // the above since the content of (interpolant *)
                // includes an additional parenthesis
                interpolant_from_file += "))\n";
                interpolant_from_file += "(check-sat)\n";
                system(("rm -rf " + temp_file_name).c_str());

                z3::solver mathsat_interpolant_parser(ctx, "QF_AUFLIA");
                mathsat_interpolant_parser.from_string(interpolant_from_file.c_str());

                auto const & interpolant_result = mathsat_interpolant_parser.assertions();
                std::cout << interpolant_result << std::endl;

                sprintf(log_command, 
                    "echo File: \"%s\" Solver Code: \"%u\" Exit Code: %d >> \"%s\"",
                    file_for_implementation.c_str(), 4, ret, file_statistics);
              }
            }
            else{
              system(("rm -rf " + temp_file_name).c_str());
              sprintf(log_command, 
                  "echo File: \"%s\" Solver Code: \"%u\" Exit Code: %d >> \"%s\"",
                  file_for_implementation.c_str(), 4, 1, file_statistics);
            }

            );

            system(("rm -rf " + temp_file).c_str());
      }
      return;
    case SMTINTERPOL:
      {
        TEMP_FILE_SETUP;
        BENCHMARK_COMMAND(
            // WRITER
            z3::solver tseitin_solver(ctx, "QF_AUFLIA");
            tseitin_solver.add(z3::mk_and(part_a));
            tseitin_solver.add(z3::mk_and(part_b));
            axdinterpolator_file 
            << "(set-option :print-success false)\n" 
            << "(set-option :produce-interpolants true)" 
            << std::endl;
            axdinterpolator_file 
            << "(set-logic QF_AUFLIA)" << std::endl;
            axdinterpolator_file << nameAssertionsZ3(tseitin_solver.to_smt2());
            axdinterpolator_file << "(get-interpolants part_a part_b)" << std::endl;
            axdinterpolator_file.close();,
            // EXEC_COMMAND
            std::string temp_file_name = "smtinterpol_inter_temp_" + current_file;
            sprintf(exec_command, 
              "java -jar ./../../bin/smtinterpol-2.5-663-gf15aa217.jar -w %s > %s",
              file_for_implementation.c_str(), temp_file_name.c_str());,
            // LOG_COMMAND
            std::ifstream result(temp_file_name.c_str());
            std::string line("");
            std::string interpolant_from_file("");
            // We consume one line because
            // mathsat outputs "check-sat" followed
            // by the interpolant
            std::getline(result, line);

            if(line == "unsat"){
              interpolant_from_file += tseitin_solver.to_smt2_decls_only();
              interpolant_from_file += "(assert \n";
              std::getline(result, line);
              interpolant_from_file += line.erase(0, 1) + "\n";
              // The following removes the last parenthesis 
              // and the extra '\n'
              interpolant_from_file.erase(interpolant_from_file.size() - 2, 2);
              // Only one parenthesis is needed to close
              // the above since the content of (interpolant *)
              // includes an additional parenthesis
              interpolant_from_file += ")\n";
              interpolant_from_file += "(check-sat)\n";
              system(("rm -rf " + temp_file_name).c_str());

              z3::solver smtinterpol_interpolant_parser(ctx, "QF_AUFLIA");
              smtinterpol_interpolant_parser.from_string(interpolant_from_file.c_str());

              auto const & interpolant_result = smtinterpol_interpolant_parser.assertions();
              std::cerr << interpolant_result << std::endl;

              sprintf(log_command, 
                  "echo File: \"%s\" Solver Code: \"%u\" Exit Code: %d >> \"%s\"",
                  file_for_implementation.c_str(), 5, ret, file_statistics);
            }
            else{
              system(("rm -rf " + temp_file_name).c_str());
              sprintf(log_command, 
                  "echo File: \"%s\" Solver Code: \"%u\" Exit Code: %d >> \"%s\"",
                  file_for_implementation.c_str(), 5, 1, file_statistics);
            }

            );

            system(("rm -rf " + temp_file).c_str());
      }
      return;
  }
}

void UAutomizerFileReader::reset(){
  line = "";
  current_frame = "";
  nesting_level = 0;
  max_nesting_level = 0;
  stack_of_frames.clear();
}

void UAutomizerFileReader::process(char const * file_path){
  std::string string_path = file_path;
  current_file = 
    string_path.substr(string_path.find_last_of("/\\") + 1);
  std::ifstream smt_file;
  smt_file.open(file_path);

  while(std::getline(smt_file, line)){

    if(isPushCmd()){
      nesting_level++;
      stack_of_frames.push_back(current_frame);
      current_frame = "";
    }
    else if(isPopCmd()){
      nesting_level--;
      if(num_samples == 0){
        smt_file.close();
        reset();
        return;
      }
      num_samples--;
      if(test_our_implementation)
        testAXDInterpolator();
      else
        testOtherSolvers();
      current_frame = stack_of_frames.back();
      stack_of_frames.pop_back();
    }
    else if(isEchoCmd())
      continue;
    else
      current_frame += "\n" + line;

    if(nesting_level < 0)
      throw 
        "Error @ "
        "UAutomizerFileReader::process "
        "The number of pop operations are bigger than "
        "the number of push operations.";

    if(max_nesting_level < nesting_level)
      max_nesting_level = nesting_level;
  }

  smt_file.close();
  reset();
}
