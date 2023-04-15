#ifndef _UTIL_H
#define _UTIL_H

#include "z3++.h"
#include <set>
#include <cstring>
#include <utility>
#include <regex>
#include <map>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>

#ifndef NDEBUG
#   define ASSERT(condition, message) \
  do { \
    if (! (condition)) { \
      std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
      << " line " << __LINE__ << ": " << message << std::endl; \
      std::terminate(); \
    } \
  } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

//#define m_out std::cerr
#define m_out std::cout

#define inSet(element, set) (set.find(element) != set.end())

namespace axdinterpolator {

  struct Z3ExprComparator {
    bool operator()(z3::expr const &a, z3::expr const &b) const;
  };

  typedef std::set<z3::expr, Z3ExprComparator> z3_expr_set;

  class z3_expr_vector_unique : public z3::expr_vector {
    std::set<unsigned> expr_ids;

    public:
    z3_expr_vector_unique(z3::context &);
    void push(z3::expr const &);
  };

  class z3_sort_vector_unique : public z3::sort_vector {
    std::set<unsigned> sort_ids;

    public:
    z3_sort_vector_unique(z3::context &);
    void push(z3::sort const &);
  };

  bool isSpaceOrParen(char);
  void extractNameFromSort(std::string &);
  bool isArraySort(z3::sort const &);

  std::ostream &operator<<(std::ostream &, 
      z3_expr_set const &);
  std::ostream &operator<<(std::ostream &, 
      std::map<unsigned, axdinterpolator::z3_expr_set> const &);

  // TODO: add utility functions

} // namespace axdinterpolator

#endif
