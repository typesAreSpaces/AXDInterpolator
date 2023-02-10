#include "_AXDInterpolant.h"
#include "util.h"

enum BENCHMARK_EXIT_CODE { SUCCESS, FAILED, TIMEOUT };

int main(int argc, char *argv[]) {

  z3::context ctx;
  ctx.set(":pp-min-alias-size", 1000000);
  ctx.set(":pp-max-depth", 1000000);

  switch (argc) {
  case 5: {

    const char *theory = argv[1];
    const char *smt_filename = argv[2];
    const char *smt_engine_code = argv[3];

    InputFormulaParser input(theory, smt_filename, smt_engine_code, ctx);

    AXDSignature sig(ctx, theory, input.getDecls());

    try {
      do {
        AXDInterpolant axd(sig,
			   // Input formulas
			   input.currentPartA(),
			   input.currentPartB(),
			   // smt2 file name
			   smt_filename);

	switch (*smt_engine_code) {
	case '0':
	  axd.z3OutputFile();
	  break;
	case '1':
	  axd.mathsatOutputFile();
	  break;
	case '2':
	  axd.smtInterpolOutputFile();
	  break;
	default:
	  std::cout << "Not valid SMT solver option." << std::endl;
	  // return 3;
	}

	std::cout << axd << std::endl;
      } while (input.next());

    } catch (char const *e) {
      std::cout << e << std::endl;
      // return 4;
    }

    return 0;
  }

  default:
    std::cout << "Invalid number of arguments." << std::endl;
    return 5;
  }
}
