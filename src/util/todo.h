#ifndef TODO_H
#define TODO_H

#include "z3++.h"

void traverse(z3::expr);

void M_O_instantiation(z3::expr, const z3::expr_vector &, const z3::expr_vector &);

void simple_test(z3::expr);

#endif /* TODO_H */
