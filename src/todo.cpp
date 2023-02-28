#include "../include/todo.h"
#include <sys/wait.h>
#include "../include/Preprocess.h"

void traverse(z3::expr e) {
  std::cout << "Current e: " << e << std::endl;
  std::cout << e.num_args() << std::endl;
  std::cout << func_name(e) << std::endl;
  if(e.is_app()){
    for(unsigned int i = 0; i < e.num_args(); i++){
      traverse(e.arg(i));
    }
  }
  return;
}

// uses z3 interface to compare expressions
bool compare_expr(z3::expr a, z3::expr b) {
  return z3::eq(a, b);
}

// creates a vector of all subformulas and atoms (where the num_args() == 0)
void flattening_helper(z3::expr e, std::vector<z3::expr> & subformulas, std::vector<z3::expr> & atoms) {
    if(e.is_app()) {
      for (unsigned int i = 0; i < e.num_args(); i++) {
	if (e.arg(i).num_args() != 0) {
	  subformulas.push_back(e.arg(i));	
	} else {
	  atoms.push_back(e.arg(i));
	}
	flattening_helper(e.arg(i), subformulas, atoms);
      }
    }
}

// generates new variable name using a counter and the name of the "parent" formula
z3::expr get_new_variable(z3::expr e, z3::context & ctx, int & counter) {
  std::string name_str = e.decl().name().str() + std::to_string(counter);
  const char * name_char = const_cast<char*>(name_str.c_str());
  z3::symbol name = ctx.str_symbol(name_char);
  auto new_var = ctx.constant(name, e.get_sort());
  counter++;
  return new_var;
}

// flattening function
// first, removes the duplicates from the subformula and atom vectors
// then, it goes through the subformulas, generating a unique variable name and replacing all occurences of that formula in other subformulas with the new variable name
// an equivalence between the subformula and its new variable name is added to the vector of formulas
// finally, the atom and converted subformulas are combined into a single vector that is returned
std::vector<z3::expr> flattening(z3::expr e, z3::context & ctx) {
  std::vector<z3::expr> subformulas;
  std::vector<z3::expr> atoms;
  flattening_helper(e, subformulas, atoms);
  subformulas.push_back(e); // need to include the full expression

  int var_name_counter = 0;

  std::vector<z3::expr> remove_duplicates_subformulas;
  std::vector<z3::expr> remove_duplicates_atoms;

  // I tried to use set and a custom comparator, but it was not working :(
  for (unsigned int i = 0; i < subformulas.size(); i++) {
    bool unique = true;
    for (unsigned int j = i; j < subformulas.size(); j++) {
      if ((i != j) && (compare_expr(subformulas[i], subformulas[j]))) {
	unique = false;
      }
    }
    if (unique) {
      remove_duplicates_subformulas.push_back(subformulas[i]);
    }
  }

  for (unsigned int i = 0; i < atoms.size(); i++) {
    bool unique = true;
    for (unsigned int j = i; j < atoms.size(); j++) {
      if ((i != j) && (compare_expr(atoms[i], atoms[j]))) {
	unique = false;
      }
    }
    if (unique) {
      remove_duplicates_atoms.push_back(atoms[i]);
    }
  }
    
  std::vector<z3::expr> final_formulas;
  
  for (unsigned int j = 0; j < remove_duplicates_subformulas.size(); j++) {
    z3::expr new_var = get_new_variable(e, ctx, var_name_counter); // generates new variable

    // creates vector with the subformula and its new variable name so we can use substitute
    z3::expr_vector I(ctx);
    I.push_back(new_var);
    z3::expr_vector constants(ctx);
    constants.push_back(remove_duplicates_subformulas[j]);

    if (j != remove_duplicates_subformulas.size() - 1) { // because we don't want to full formula to be assigned a variable
      remove_duplicates_subformulas[j] = (new_var == remove_duplicates_subformulas[j]);
    }

    // looks through all of the subformula and tries to replace current formula with the new variable
    for (unsigned int k = 0; k < remove_duplicates_subformulas.size(); k++) {
      if (j != k) {
	remove_duplicates_subformula
	s[k] = remove_duplicates_subformulas[k].substitute(constants, I);
      }
    }
  }

  // adds all converted subformulas to final formulas vector 
  for (unsigned int i = 0; i < remove_duplicates_subformulas.size(); i++) {
    final_formulas.push_back(remove_duplicates_subformulas[i]);
  }
  
  // adds all atoms to final formulas vector 
  for (unsigned int i = 0; i < remove_duplicates_atoms.size(); i++) {
    final_formulas.push_back(remove_duplicates_atoms[i]);
  }

  // Here
  // How to check equisatisfiability
  // = t1 t2

  // solver1.add(t1 == t2)
  // solver1.check()
  // flattening(t1) -> {(eq1, ..., eq_m) t1'}
  // flattening(t2) -> {(eq1', ..., eq_m') t2'}
  // solver2.add(eq1)
  //...
  // solver2.add(eq_m)
  // solver2.add(eq1')
  //...
  // solver2.add(eq_m')
  // solver2.add(t1' == t2')
  // solver2.check()

  return final_formulas;
}

void M_O_instantiation(z3::expr f, const z3::expr_vector &I,
		       const z3::expr_vector &constants) {
  assert(I.size() == constants.size());
  std::cout << "Print1: ";
  std::cout << f << std::endl;
  std::cout << "Print2: ";
  std::cout << f.substitute(constants, I) << std::endl;
}

/*void initSaturation() {
  for(auto const & _4tuple : write_vector.m_vector) {
    auto const & a = std::get<0>(_4tuple);
    auto const & b = std::get<1>(_4tuple);
    auto const & i = std::get<2>(_4tuple);
    auto const & e = std::get<3>(_4tuple);

    auto const & curr_rd = sig.getRdBySort(a.get_sort());

    part_2.push_back(
		     z3::implies(z3::and(e != bottom,
					 0 <= i,
					 i <= len(b)), curr_rd(a, i) == e)
		     );

    part_2.push_back(
		     z3::implies(z3::or(i < 0,
					i > len(b),
					e == bottom)), curr_rd(a, i) ==
  curr_rd(b, i));
  }
  }*/
