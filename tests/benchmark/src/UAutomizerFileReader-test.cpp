#include "UAutomizerFileReader.h"

void UAutomizerFileReader::testAXDInterpolator() const {

  TEMP_FILE_SETUP;
  BENCHMARK_COMMAND(
      z3::solver tseitin_solver(ctx, "QF_AUFLIA");
      tseitin_solver.add(z3::mk_and(part_a));
      tseitin_solver.add(z3::mk_and(part_b));

      axdinterpolator_file << tseitin_solver.to_smt2();
      axdinterpolator_file.close();,

      sprintf(exec_command,
        "./../../bin/axd_interpolator QF_LIA %s %u 1000000;",
        file_for_implementation.c_str(), curr_solver);,
#if REPORT_BAD_CASES
      if(ret != 0 && ret != 152){
      char complain_command[1000];
      sprintf(
          complain_command, 
          "echo File: \"%s\" Solver Code: \"%u\" Sample Id: %d Exit Code: %d >> /home/jose/bad_cases.txt",
          file_for_implementation.c_str(), curr_solver, 500 - num_samples, ret);
      system(complain_command);
      system(("mv " 
            + file_for_implementation 
            + " ~/" + file_for_implementation + std::to_string(500 - num_samples)).c_str());
      }
#endif
      sprintf(log_command, 
        "echo File: \"%s\" Solver Code: \"%u\" Exit Code: %d >> \"%s\"",
        file_for_implementation.c_str(), curr_solver, ret, file_statistics);
      );
      system(("rm -rf " + temp_file).c_str());
}

void UAutomizerFileReader::testOtherSolvers() {

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
