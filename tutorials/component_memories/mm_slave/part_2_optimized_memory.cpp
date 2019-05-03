// See README for a description of this tutorial
#include "HLS/hls.h"
#include <stdio.h> // For printf
#include <stdlib.h> // For rand

#define NROW 4
#define NCOL 8

component void reduce_sum(hls_avalon_slave_memory_argument(NCOL*NROW*sizeof(int))
                          hls_bankbits(3,4) int *sized_buffer) {
  #pragma unroll 1
  for (int col = 1; col < NCOL; col++)
    #pragma unroll
    for (int row = 0; row < NROW; row++)
      // Access in 2D: sized_buffer[row][0] += sized_buffer[row][col];
      sized_buffer[NCOL * row] += sized_buffer[NCOL * row + col];
}

int main() {
  int buffer[NROW * NCOL];
  int reference[NROW] = {};
  // generate input data and compute the desired output
  srand(0);
  for (int row = 0; row < NROW; row++)
    for (int col = 0; col < NCOL; col++) {
      int data = rand() % 100;
      buffer[NCOL * row + col] = data;
      reference[row] += data;  
    }
  // run the component
  reduce_sum(buffer);
  // check the result
  bool pass = true;
  for (int row = 0; row < NROW; row++){
    if (buffer[NCOL * row] == reference[row]) continue; 
    printf("ERROR: row = %d, %d != %d\n", row, buffer[NCOL * row], reference[row]);
    pass = false;
  }
  if (pass)
    printf("PASSED\n");
  else
    printf("FAILED\n");
  return 0;
}
