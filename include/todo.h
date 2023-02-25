#ifndef TODO_H
#define TODO_H

#include <z3++.h>

void traverse(z3::expr);

void M_O_instantiation(z3::expr, const z3::expr_vector &,
                       const z3::expr_vector &);

bool compare_expr(z3::expr, z3::expr);

void flattening_helper(z3::expr, std::vector<z3::expr> &,
                       std::vector<z3::expr> &);

z3::expr get_new_variable(z3::expr, z3::context &, int &);

std::vector<z3::expr> flattening(z3::expr, z3::context &);

#endif /* TODO_H */
