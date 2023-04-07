#ifndef TODO_H
#define TODO_H

#include <vector>
#include <z3++.h>

void traverse(z3::expr);

void M_O_instantiation(z3::expr, const z3::expr_vector &,
                       const z3::expr_vector &);

bool compare_expr(z3::expr, z3::expr);

void flattening_helper(z3::expr, z3::expr_vector &,
                       z3::expr_vector &);

z3::expr get_new_variable(z3::expr, z3::context &, int &);

z3::expr_vector flattening(z3::expr, z3::context &);


void simple_test(z3::expr);
void simple_test2();

#endif /* TODO_H */
