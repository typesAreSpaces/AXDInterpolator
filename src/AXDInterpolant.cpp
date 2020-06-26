#include "AXDInterpolant.h"

AXDInterpolant::AXDInterpolant(z3::context & ctx, z3::expr const & a, z3::expr const & b) : 
  AXDSignature(ctx),
  ctx(ctx),
  solver(ctx),
  part_a({z3::expr_vector(ctx), z3::expr_vector(ctx)}), 
  part_b({z3::expr_vector(ctx), z3::expr_vector(ctx)}),
  fresh_index(0),
  common_array_vars(ctx), 
  common_array_var_names({}), aux_common_array_var_names({})
{
  initial_split(a, part_a);
  initial_split(b, part_b);

  traverseA(a);
  traverseB(b);
  aux_common_array_var_names.clear();

#if _DEBUG_AXD_INTER_
  for(auto const & common_name : common_array_var_names)
    std::cout << common_name << std::endl;
#endif

  for(auto const & form : part_a.second)
    solver.add(form);
  for(auto const & form : part_b.second)
    solver.add(form);
  //loop();
}

void AXDInterpolant::initial_split(z3::expr const & e, Part & part){
  if(e.is_app()){
    switch(e.decl().decl_kind()){
      case Z3_OP_AND:
        {
          unsigned num_args = e.num_args();
          for(unsigned i = 0; i < num_args; ++i)
            initial_split(e.arg(i), part);
          return;
        }
      case Z3_OP_EQ:
      case Z3_OP_DISTINCT:
        {
          auto const & lhs = e.arg(0);
          auto const & rhs = e.arg(1);
          auto const & lhs_range = lhs.decl().range();
          auto const & rhs_range = rhs.decl().range();

          // Case for (dis)equality between ELEM  to part.second
          if(lhs_range.to_string() == element_sort.to_string()
              && rhs_range.to_string() == element_sort.to_string()){
            part.second.push_back(e);
            return;
          }
          // Case for (dis)equality between ARRAY to part.first
          if(lhs_range.to_string() == array_sort.to_string() 
              && rhs_range.to_string() == array_sort.to_string()){
            part.first.push_back(e);
            return;
          }
          // Case for (dis)equality between INDEX to part.second
          // but if the func term diff or wr is used then to part.first
          if(lhs_range.to_string() == index_sort.to_string() 
              && rhs_range.to_string() == index_sort.to_string()){
            if(lhs.decl().name() == diff.name() || rhs.decl().name() == diff.name()
                || lhs.decl().name() == wr.name() || rhs.decl().name() == wr.name()){
              part.first.push_back(e);
              return;
            }
            part.second.push_back(e);
            return;
          }
          // Case for (dis)equality between INT   to part.second
          // but if the func term diff or wr is used then to part.first
          if(lhs_range.to_string() == int_sort.to_string() 
              && rhs_range.to_string() == int_sort.to_string()){
            if(lhs.decl().name() == diff.name() || rhs.decl().name() == diff.name()
                || lhs.decl().name() == wr.name() || rhs.decl().name() == wr.name()){
              part.first.push_back(e);
              return;
            }
            part.second.push_back(e);
            return;
          }
          throw "Not a well-formed/valid equality";
        }
      case Z3_OP_LE:
      case Z3_OP_GE:
      case Z3_OP_LT:
      case Z3_OP_GT:
        // Case for INDEX to part.second
        // Case for INT   to part.second
        part.second.push_back(e);
        return;
      default:
        throw "A formula in part_a is not in AXD";
    }
  }
  throw "A formula in part_a "
    "is not in function application";
}

void AXDInterpolant::traverseA(z3::expr const & e){
  if(e.is_app()){
    unsigned num_args = e.num_args();
    if(num_args == 0){
      if(e.decl().range().to_string() == array_sort.to_string())
        aux_common_array_var_names.insert(e.decl().name().str());
      return;
    }
    for(unsigned i = 0; i < num_args; ++i)
      traverseA(e.arg(i));
    return;
  }
  throw "The formula is not"
    "a function application";
}

void AXDInterpolant::traverseB(z3::expr const & e){
  if(e.is_app()){
    unsigned num_args = e.num_args();
    if(num_args == 0){
      if(e.decl().range().to_string() == array_sort.to_string()){
        auto name = e.decl().name().str();
        if(inSet(name, aux_common_array_var_names))
          common_array_var_names.insert(name);
      }
      return;
    }
    for(unsigned i = 0; i < num_args; ++i)
      traverseB(e.arg(i));
    return;
  }
  throw "The formula is not"
    "a function application";
}

void AXDInterpolant::saturate(Part & part, z3::expr const & new_formula){
  part.first.push_back(new_formula);
  // TODO: implement the rest
}

void AXDInterpolant::loop(){
  unsigned stop;
  while(solver.check() != z3::unsat){
    std::cout << solver.check() << std::endl;
    std::cin >> stop;
    // TODO: implement the rest
  }
}

std::ostream & operator << (std::ostream & os, AXDInterpolant const & axd_interp){
  os << "Part a 1" << std::endl;
  os << axd_interp.part_a.first << std::endl;
  os << "Part a 2" << std::endl;
  os << axd_interp.part_a.second << std::endl;
  os << "Part b 1" << std::endl;
  os << axd_interp.part_b.first << std::endl;
  os << "Part b 2" << std::endl;
  os << axd_interp.part_b.second;
  return os;
}
