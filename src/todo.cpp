#include "todo.h"
#include <sys/wait.h>

void traverse(z3::expr e) {
  std::cout << "Current e: " << e << std::endl;
  if(e.is_app()){
    for(unsigned int i = 0; i < e.num_args(); i++){
      traverse(e.arg(i));
    }
  }
  return;
}

void test() {
  std::cout << ">> Test" << std::endl;
}

void M_O_instantiation(z3::expr f, const z3::expr_vector & I, const z3::expr_vector & constants) {
  assert (I.size() == constants.size());
  std::cout << "Print1: ";
  std::cout << f << std::endl;
  std::cout << "Print2: ";
  std::cout << f.substitute(constants, I) << std::endl;
}
