#include "AXDSignature.h"
#include "z3++.h"
#include <sstream>

#define INDEX_UNDEFINED_WR_RD(ELEMENT_SORT)\
  undefined_es.push_back(ctx.constant(("undefined" + temp_name_sort).c_str(), \
        ELEMENT_SORT));\
        wr_es.push_back(ctx.function(("wr" + temp_name_sort).c_str(),\
              curr_array_sort, int_sort, ELEMENT_SORT, curr_array_sort));\
              rd_es.push_back(ctx.function(("rd" + temp_name_sort).c_str(),\
                    curr_array_sort, int_sort, ELEMENT_SORT));

AXDSignature::AXDSignature(
    z3::context & ctx, 
    char const * theory_string,
    std::string decls) :
  ctx(ctx),
  theory_name(QF_TO),

  bool_sort(ctx.bool_sort()), 
  int_sort(ctx.int_sort()), 

  element_sorts(ctx),
  array_sorts(ctx),
  undefined_es(ctx),
  empty_array_es(ctx),
  diff_es(ctx),
  diff_k_es(ctx),
  wr_es(ctx),
  rd_es(ctx),
  length_es(ctx),
  array_sort_map({})
{
  if(!strcmp(theory_string, "QF_TO"))
    theory_name = QF_TO;
  else if(!strcmp(theory_string, "QF_IDL"))
    theory_name = QF_IDL;
  else if(!strcmp(theory_string, "QF_UTVPI"))
    theory_name = QF_UTVPI;
  else if(!strcmp(theory_string, "QF_LIA"))
    theory_name = QF_LIA;
  else
    throw 
      "Error @ "
      "AXDSignature constructor. "
      "theory not supported";

  processArrayDecls(decls);
  indexElementSorts();
}

bool AXDSignature::isSpaceOrParen(char T){
  return T == ' ' || T == '(' || T == ')';
}

void AXDSignature::extractNameFromSort(std::string & s) const {
  s.erase(
      std::remove_if(s.begin(), s.end(), isSpaceOrParen), 
      s.end());
}

void AXDSignature::processArrayDecls(std::string decls){
  std::smatch match;
  std::regex array_decl_regex(
      "\\(declare-fun .* \\(\\) \\(Array.*\\)\\)");
  while(regex_search(decls, match, array_decl_regex)){
    auto const & curr_decl = match.str(0);
    std::istringstream iss(curr_decl);
    std::string curr_name;
    iss >> curr_name >> curr_name;
    z3::expr temp_expr = ctx.parse_string(
        (curr_decl + " (assert (= " + curr_name + " " + curr_name + "))").c_str());

    element_sorts.push(temp_expr.arg(0).get_sort().array_range());
    decls = match.suffix().str();
  }

  z3::sort_vector stack(ctx);
  for(auto const & elem : element_sorts)
    if(elem.is_array())
      stack.push_back(elem);
    
  // Include the range sort recursively
  // of all array sorts
  while(stack.size() != 0){
    auto const & curr_sort = stack.back();
    stack.pop_back();
    if(curr_sort.is_array()){
      element_sorts.push(curr_sort.array_range());
      stack.push_back(curr_sort.array_range());
    }
  }
}

void AXDSignature::indexElementSorts(){
  // [NOTICE]: curr_element_sort can be an Array type
  for(auto const & curr_element_sort : element_sorts){

    std::string temp_name_sort = curr_element_sort.to_string();
    extractNameFromSort(temp_name_sort);

    auto const & curr_array_sort = 
      ctx.uninterpreted_sort(("ArraySort" +  temp_name_sort).c_str());

    array_sort_map.insert(
        std::pair<unsigned, unsigned>(
          curr_element_sort.id(), array_sorts.size()));
    array_sort_map.insert(
        std::pair<unsigned, unsigned>(
          curr_array_sort.id(), array_sorts.size()));

    array_sorts.push_back(curr_array_sort);

    empty_array_es.push_back(ctx.constant(("empty_array" + temp_name_sort).c_str(), 
          curr_array_sort));
    diff_es.push_back(ctx.function(("diff" + temp_name_sort).c_str(), 
          curr_array_sort, curr_array_sort, int_sort));
    diff_k_es.push_back(ctx.function(("diff_" + temp_name_sort).c_str(), 
          int_sort, curr_array_sort, curr_array_sort, int_sort));
    length_es.push_back(ctx.function(("length" + temp_name_sort).c_str(), 
          curr_array_sort, int_sort));
  }

  for(auto const & curr_element_sort : element_sorts){
    std::string temp_name_sort = curr_element_sort.to_string();
    extractNameFromSort(temp_name_sort);

    auto const & curr_array_sort = 
      ctx.uninterpreted_sort(("ArraySort" +  temp_name_sort).c_str());

    if(curr_element_sort.is_array()){
      auto const & abstract_element_sort = 
        getArraySortBySort(curr_element_sort.array_range());
      INDEX_UNDEFINED_WR_RD(abstract_element_sort);
    }
    else{
      INDEX_UNDEFINED_WR_RD(curr_element_sort);
    }
  }
}

bool AXDSignature::is_QF_TO() const {
  return theory_name == QF_TO;
}

bool AXDSignature::is_QF_IDL() const {
  return theory_name == QF_IDL;
}

bool AXDSignature::isArraySort(z3::sort const & s) const {
  return s.to_string().find("ArraySort") != std::string::npos;
}

void AXDSignature::setTheory(TheoryName new_theory_name){
  theory_name = new_theory_name;
}

std::ostream & operator << (
    std::ostream & out, 
    AXDSignature::z3_expr_set const & collection){
  for(auto const & x : collection)
    out << x << std::endl;
  return out;
}

std::ostream & operator << (
    std::ostream & os,
    AXDSignature::TheoryName const & theory_name){
  switch(theory_name){
    case AXDSignature::QF_TO:
      return os << "QF_TO";
    case AXDSignature::QF_IDL:
      return os << "QF_IDL";
    case AXDSignature::QF_UTVPI:
      return os << "QF_UTVPI";
    case AXDSignature::QF_LIA:
      return os << "QF_LIA";
  }
  return os;
}
