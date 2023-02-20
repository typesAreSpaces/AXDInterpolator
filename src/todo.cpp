#include "todo.h"
#include <sys/wait.h>
#include "Preprocess.h"

void traverse(z3::expr e) {
  std::cout << ">> Current e: " 
    << e << std::endl;
  std::cout << ">> Num of args: "
    << e.num_args() << std::endl;
  std::cout << ">> Print func name: "
    << func_name(e) << std::endl;
  if(e.is_app()){
    for(unsigned int i = 0; i < e.num_args(); i++){
      traverse(e.arg(i));
    }
  }
  return;
}

void M_O_instantiation(z3::expr f, const z3::expr_vector & I, const z3::expr_vector & constants) {
  assert (I.size() == constants.size());
  std::cout << "Print1: ";
  std::cout << f << std::endl;
  std::cout << "Print2: ";
  std::cout << f.substitute(constants, I) << std::endl;
}

void initSaturation() {
  //for(auto const & _4tuple : write_vector.m_vector) {
  //auto const & a = std::get<0>(_4tuple);
  //auto const & b = std::get<1>(_4tuple);
  //auto const & i = std::get<2>(_4tuple);
  //auto const & e = std::get<3>(_4tuple);

  //auto const & curr_rd = sig.getRdBySort(a.get_sort());

  //part_2.push_back(
  //z3::implies(z3::and(e != bottom,
  //0 <= i,
  //i <= len(b)), curr_rd(a, i) == e)
  //);

  //part_2.push_back(
  //z3::implies(z3::or(i < 0,
  //i > len(b),
  //e == bottom)), curr_rd(a, i) == curr_rd(b, i));
  //}
}
