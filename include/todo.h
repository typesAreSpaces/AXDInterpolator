#ifndef TODO_H
#define TODO_H

#include <z3++.h>

void traverse(z3::expr);

void test();

void M_O_instantiation(z3::expr, const z3::expr_vector &, const z3::expr_vector &);

#endif /* TODO_H */
