#include "UAutomizerFileReader.h"
#include <sys/stat.h>

inline bool exists_file (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

int main(int argc, char * argv[]){

  if(argc != 2){
    std::cerr << "Invalid number of inputs.\n";
    return 0;
  }

  char const * file = argv[1];
  if(!exists_file(file)){
    std::cerr << "File doesnt exists.\n";
    return 0;
  }

  UAutomizerFileReader reader;
  reader.process(file);

  return 0;
}
