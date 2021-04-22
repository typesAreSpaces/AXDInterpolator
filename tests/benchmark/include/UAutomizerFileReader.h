#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>
#include <cstring>
#include <string>
#include <cstdlib>
#include <z3++.h>

class UAutomizerFileReader {

  std::string line, current_frame, current_file;
  int nesting_level, max_nesting_level;
  std::vector<std::string> stack_of_frames;

  bool isPushCmd() const;
  bool isPopCmd() const;
  bool isEchoCmd() const; 

  void action() const; 
  void reset(); 

  public:
  UAutomizerFileReader();
    
  void process(char const *);
};
