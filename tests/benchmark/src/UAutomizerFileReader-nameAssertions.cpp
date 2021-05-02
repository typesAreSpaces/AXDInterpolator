#include "UAutomizerFileReader.h"

std::string UAutomizerFileReader::nameAssertionsZ3(std::string const & s) const {
  std::stringstream decl_strm(s);
  std::string curr_line, collected_assertion = "", ret = "";
  bool matching_assert = false, matching_part_a = true;
  unsigned num_balanced_paren = 0;

  while(std::getline(decl_strm, curr_line)){

    if(curr_line.find("assert") != std::string::npos)
      matching_assert = true;

    if(matching_assert){
      for(auto const & c : curr_line){
        if(c == '(')
          num_balanced_paren++;
        if(c == ')')
          num_balanced_paren--;
      }

      collected_assertion += curr_line;
      if(num_balanced_paren == 0){
        matching_assert = false;
        collected_assertion = collected_assertion.substr(8);
        collected_assertion.pop_back();
        if(matching_part_a){
          collected_assertion = "(assert (! " + collected_assertion +" :named part_a))";
          matching_part_a = false;
        }
        else
          collected_assertion = "(assert (! " + collected_assertion +" :named part_b))";

        ret += collected_assertion + "\n";
        collected_assertion = "";
      }
    }
    else
      ret += curr_line + "\n";
  }
  return ret;
}

std::string UAutomizerFileReader::nameAssertionsMathsat(std::string const & s) const {
  std::stringstream decl_strm(s);
  std::string curr_line, collected_assertion = "", ret = "";
  bool matching_assert = false, matching_part_a = true;
  unsigned num_balanced_paren = 0;

  while(std::getline(decl_strm, curr_line)){

    if(curr_line.find("assert") != std::string::npos)
      matching_assert = true;

    if(matching_assert){
      for(auto const & c : curr_line){
        if(c == '(')
          num_balanced_paren++;
        if(c == ')')
          num_balanced_paren--;
      }

      collected_assertion += curr_line;
      if(num_balanced_paren == 0){
        matching_assert = false;
        collected_assertion = collected_assertion.substr(8);
        collected_assertion.pop_back();
        if(matching_part_a){
          collected_assertion = "(assert (! " + collected_assertion +" :interpolation-group part_a))";
          matching_part_a = false;
        }
        else
          collected_assertion = "(assert (! " + collected_assertion +" :interpolation-group part_b))";

        ret += collected_assertion + "\n";
        collected_assertion = "";
      }
    }
    else
      ret += curr_line + "\n";
  }
  return ret;
}
