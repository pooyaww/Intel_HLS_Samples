// See README for a description of this tutorial
#include "HLS/hls.h"
#include <stdio.h> // For printf

#define ARR_SIZE 600
#define SUM_LENGTH 4

static const int hls_doublepump a[ARR_SIZE] = {0,1,2,3,4,5,6,7,8,9};

component int partial_sum(int x){
  int rdata = 0;
  #pragma unroll
  for (int i = 0; i < SUM_LENGTH; i++){
    rdata += a[x+i];
  }
  return rdata;
}

int main(){
  bool pass = true;
  for(int i = 0; i <= ARR_SIZE - SUM_LENGTH; i++){
    // compute the desired output, use it as reference
    int reference = 0;
    for (int j = 0; j < SUM_LENGTH; j++)
      reference += a[i + j];
    // run the component
    int rdata = partial_sum(i);
    // check the result
    bool data_okay = (rdata == reference);
    pass &= data_okay;

    if (!data_okay){
      printf("ERROR: i = %d, %d != %d\n", i, rdata, reference);
    }
  }

  if(pass){
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }
  return 0;
}
