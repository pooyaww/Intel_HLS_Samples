#include "HLS/hls.h"
#include <stdio.h> // printf
#include <stdlib.h> // srand, rand

#define NUM_TESTS 1000
#define SEED 0

component void mult(ihc::stream_in<int> &a, ihc::stream_in<int> &b, ihc::stream_out<int> &c) {
  int data = a.read() * b.read();
  c.write(data);
}

int main() {
  ihc::stream_in<int> a, b;
  ihc::stream_out<int> c;
  int a_data[NUM_TESTS], b_data[NUM_TESTS], c_expected[NUM_TESTS];
  int fail_count = 0;

  srand(SEED);

  // populate input streams with test data
  for (int i = 0; i < NUM_TESTS; i++) {
    a_data[i] = rand();
    b_data[i] = rand();
    c_expected[i] = a_data[i] * b_data[i];
    a.write(a_data[i]);
    b.write(b_data[i]);
  }

  // run component
  for (int i = 0; i < NUM_TESTS; i++) {
    mult(a, b, c);

    // verify output
    int c_result = c.read();
    if (c_result != c_expected[i]) {
      printf("ERROR (case %d): %d != %d", i, c_result, c_expected[i]);
      fail_count++;
    }
  }

  // print final test results
  if (fail_count == 0) {
    printf("PASSED\n");
  } else {
    printf("FAILED (%d incorrect results)\n", fail_count);
  }

  return 0;
}
