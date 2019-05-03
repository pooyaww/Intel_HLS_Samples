// See README for a description of this tutorial
#include <stdio.h>
#include <math.h>
#include "HLS/hls.h"
#include "HLS/ac_int.h"

using namespace ihc;

#define TEST_SIZE 1024
#define TEST_SIZE_BIT_WIDTH 11 
#define MAX_DATA_BIT_WIDTH 24
#define R_MASK 0xFF0000
#define G_MASK 0x00FF00
#define B_MASK 0x0000FF
#define SEED 4

component void image_add(
    ac_int<MAX_DATA_BIT_WIDTH,false> *a,
    ac_int<MAX_DATA_BIT_WIDTH,false> *b,
    ac_int<MAX_DATA_BIT_WIDTH,false> *c,
    ac_int<TEST_SIZE_BIT_WIDTH,false> N)
{
  for (ac_int<TEST_SIZE_BIT_WIDTH,false> i = 0; i < N; ++i)
  {
    int tmp = a[i] & R_MASK + b[i] & R_MASK;
    c[i] = tmp < R_MASK ? tmp : R_MASK;

    tmp = (a[i] & G_MASK) + (b[i] & G_MASK);
    c[i] += tmp < G_MASK ? tmp : G_MASK;

    tmp = (a[i] & B_MASK) + (b[i] & B_MASK);
    c[i] += tmp < B_MASK ? tmp : B_MASK;

  }
}

int main(void) {

  ac_int<MAX_DATA_BIT_WIDTH,false> A[TEST_SIZE];
  ac_int<MAX_DATA_BIT_WIDTH,false> B[TEST_SIZE];
  ac_int<MAX_DATA_BIT_WIDTH,false> C[TEST_SIZE];

  // Prepare the input data
  srand(SEED);
  for (int i = 0; i < TEST_SIZE; ++i) {
    A[i] = rand() & 0xfff;
    B[i] = rand() & 0xfff;
  }


  // Run the component
  image_add(A, B, C, TEST_SIZE);

  // Check the output
  bool passed = true;
  for (int i = 0; i < TEST_SIZE; ++i) {

    int tmp = (A[i] & R_MASK) + (B[i] & R_MASK);
    bool data_okay = (C[i] & R_MASK) == (tmp < R_MASK ? tmp : R_MASK);
    tmp = (A[i] & G_MASK) + (B[i] & G_MASK);
    data_okay &= (C[i] & G_MASK) == (tmp < G_MASK ? tmp : G_MASK);
    tmp = (A[i] & B_MASK) + (B[i] & B_MASK);
    data_okay &= (C[i] & B_MASK) == (tmp < B_MASK ? tmp : B_MASK);

    passed &= data_okay;
    if (!data_okay) {
      printf("ERROR: C[%d] = (%d, %d, %d) != (%d, %d, %d) + (%d, %d, %d)\n", i,
          (int) C[i] & R_MASK, (int) C[i] & G_MASK, (int) C[i] & B_MASK,
          (int) A[i] & R_MASK, (int) A[i] & G_MASK, (int) A[i] & B_MASK,
          (int) B[i] & R_MASK, (int) B[i] & G_MASK, (int) B[i] & B_MASK);
    }
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}

