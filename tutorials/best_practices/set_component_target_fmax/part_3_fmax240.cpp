#include "HLS/hls.h"
#include <iostream>

unsigned int BKDRHashGolden(const char* str, unsigned int length){
  unsigned int seed = 1313; /* 31 131 1313 13131 131313 etc.. */
  unsigned int hash = 0;
  unsigned int i    = 0;
  for (i = 0; i < length; ++str, ++i) {
    hash = (hash * seed) + (*str);
  }
  return hash;
}

hls_scheduler_target_fmax_mhz(240)
component 
unsigned int BKDRHash(const char* str, unsigned int length)
{
  unsigned int seed = 1313; /* 31 131 1313 13131 131313 etc.. */
  unsigned int hash = 0;
  unsigned int i    = 0;
  for (i = 0; i < length; ++str, ++i) {
    hash = (hash * seed) + (*str);
  }
  return hash;
}

int main(){
  const int input_set_size = 6;
  const std::string input_string[input_set_size] = {
    "VwEEB",
    "P7pX1",
    "jgwkwWV4vc",
    "Hja9fr3u4x",
    "qr6KUBBmLtVUlX9",
    "si9NUUs6ghvcxBj"
  };
  for(int i = 0; i < input_set_size; i++){
    unsigned int r1 = BKDRHashGolden(input_string[i].c_str(), input_string[i].size());
    unsigned int r2 = BKDRHash(input_string[i].c_str(), input_string[i].size());
    if(r1 != r2){
      std::cout << "ERROR\n";
      return 1;
    }
  }
  std::cout << "PASSED\n";
  return 0;
}