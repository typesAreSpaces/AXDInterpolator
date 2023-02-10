#include "todo.h"

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
